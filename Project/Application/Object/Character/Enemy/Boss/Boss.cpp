#include "Boss.h"
#include "../../../../../Engine/Collision/Extrusion.h"

#include "../../../Obstacle/BaseObstacle.h"
#include "../../../../../Engine/3D/ModelDraw.h"
#include "../../../../../Engine/Physics/Gravity.h"
#include "../../../../../externals/imgui/imgui.h"
#include "../../../../../Engine/GlobalVariables/GlobalVariables.h"

#include "../../../Engine/Object/BaseObjectManager.h"

#include "../../../Engine/Math/Ease.h"
#include "../../../Engine/Math/RandomEngine.h"
LevelData::MeshData Boss::BossCreate()
{

	LevelData::MeshData data;
	static size_t id;
	// 名前
	data.name = "Boss" + std::to_string(id++);
	// トランスフォーム
	data.transform = {
		1.0f,1.0f,1.0f,
		0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f
	};

	// ファイルの名前
	data.flieName = "BossBody.obj";
	// ディレクトリパス
	data.directoryPath = "Resources/Model/Boss/";
	// クラスの名前
	data.className = "Boss";
	// 親の名前
	data.parentName = "";

	// コライダー(一時的なもの、親部分はヌルにしとく)
	OBB obb;
	obb.Initialize({ 0.0f,0.0f,0.0f }, Matrix4x4::MakeIdentity4x4(), { 1.0f,2.5f,1.0f }, static_cast<Null*>(nullptr));
	data.collider = obb;

	return data;

}

void Boss::Initialize(LevelData::MeshData* data)
{


	MeshObject::Initialize(data);

	//衝突属性(自分)
	collisionAttribute_ = 0x00000100;

	// 衝突マスク(相手)
	collisionMask_ = 0xfffffeff;

	OBB obb = std::get<OBB>(*collider_.get());
	obb.SetParentObject(this);
	obb.SetCollisionAttribute(collisionAttribute_);
	obb.SetCollisionMask(collisionMask_);
	ColliderShape* colliderShape = new ColliderShape();
	*colliderShape = obb;
	collider_.reset(colliderShape);

	// hp
	//initHp_ = 3;

	isDead_ = false;

	worldTransform_.transform_.rotate.x = 0;

	prePosition_ = worldTransform_.GetWorldPosition();

	// 初期設定
	material_->SetEnableLighting(BlinnPhongReflection);

	//RegistrationGlobalVariables();
	//ApplyGlobalVariables();

	//hp_ = initHp_;

	//初期ステート
	state_ = std::bind(&Boss::Spawn, this);

	worldTransform_.transform_.translate = oridinSpownPos_;
	worldTransform_.transform_.rotate = { 0,3.141592f,0.0f };
	worldTransform_.UpdateMatrix();

	rightArmJointWorldTransform_.Initialize();
	rightArmJointWorldTransform_.SetParent(&worldTransform_);

	leftArmJointWorldTransform_.Initialize();
	leftArmJointWorldTransform_.SetParent(&worldTransform_);

	headJointWorldTransform_.Initialize();
	headJointWorldTransform_.SetParent(&worldTransform_);

}

void Boss::Update()
{

#ifdef _DEMO
	//ApplyGlobalVariables();


#endif // _DEBUG

	MeshObject::Update();

	state_();

	worldTransform_.UpdateMatrix();

	rightArmJointWorldTransform_.UpdateMatrix();
	leftArmJointWorldTransform_.UpdateMatrix();
	headJointWorldTransform_.UpdateMatrix();

	// コライダー
	ColliderUpdate();

	//isCollision_ = false;
}

void Boss::Draw(BaseCamera& camera)
{

	MeshObject::Draw(camera);

}

void Boss::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	/*if (std::holds_alternative<Block*>(colliderPartner)) {
		if (std::get<Block*>(colliderPartner)->GetIsAttack() && !isHitCoolTime_) {
			hp_--;
			state_ = std::bind(&Boss::Damage, this);
			countUp_ = 0;
		}
		OnCollisionObstacle(colliderPartner, collisionData);
	}
	*/
}

void Boss::ParticleDraw(BaseCamera& camera)
{

}


void Boss::ColliderUpdate()
{

	OBB obb = std::get<OBB>(*collider_.get());

	float coliderAddY = 1.0f;

	obb.center_ = worldTransform_.GetWorldPosition();
	obb.center_.y += obb.size_.y / 2.0f + coliderAddY;
	obb.SetOtientatuons(worldTransform_.rotateMatrix_);

	ColliderShape* colliderShape = new ColliderShape();

	*colliderShape = obb;

	collider_->swap(*colliderShape);

}


void Boss::OnCollisionObstacle(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	BaseObstacle* obstacle = std::get<Block*>(colliderPartner);

	OBB* myOBB = &std::get<OBB>(*collider_);
	OBB* partnerOBB = &std::get<OBB>(*obstacle->GetCollider());

	float myY = myOBB->center_.y - myOBB->size_.y;
	float partnerY = partnerOBB->center_.y + partnerOBB->size_.y;

	Vector3 extrusion = {};

	if (partnerY - myY <= fabsf(Gravity::Execute().y) + 0.01f) {
		extrusion.y = partnerY - myY;
	}
	else {
		extrusion = Extrusion::OBBAndOBB(myOBB, partnerOBB);
	}

	if (extrusion.y > 0.0f) {
		//velocity_.y = 0.0f;
	}

	worldTransform_.transform_.translate += extrusion;
	worldTransform_.UpdateMatrix();

	rightArmJointWorldTransform_.UpdateMatrix();
	leftArmJointWorldTransform_.UpdateMatrix();
	headJointWorldTransform_.UpdateMatrix();

	// コライダー
	ColliderUpdate();

	//isCollision_ = true;
}

/*
void Boss::ApplyGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Boss";

	initHp_ = static_cast<uint32_t>(globalVariables->GetIntValue(groupName, "initHp"));
	runningSpeed_ = globalVariables->GetFloatValue(groupName, "runningSpeed");
	oridinalScale_ = globalVariables->GetVector3Value(groupName, "scale");
	shotFrame_ = static_cast<uint32_t>(globalVariables->GetIntValue(groupName, "shotFrame"));
	threewayRotate_ = globalVariables->GetFloatValue(groupName, "threewayRotate");

}

void Boss::RegistrationGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Boss";

	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "initHp", static_cast<int32_t>(initHp_));
	globalVariables->AddItem(groupName, "runningSpeed", runningSpeed_);
	globalVariables->AddItem(groupName, "scale", worldTransform_.transform_.scale);
	globalVariables->AddItem(groupName, "shotFrame", static_cast<int32_t>(shotFrame_));
	globalVariables->AddItem(groupName, "threewayRotate", threewayRotate_);
}
*/

void Boss::Root() {
	if (countUp_ == 0) {
		ChacePlayerY();
	}
	if (countUp_ <= 60) {
		float t = float(countUp_) / float(60);
		worldTransform_.transform_.translate.y = Ease::Easing(Ease::EaseName::Lerp, moveFromY_, moveTargetY_, t);
	}
	RotateToPlayer();
	//worldTransform_.transform_.translate = { 0,0,32.0f };
	//worldTransform_.usedDirection_ = false;
	//worldTransform_.transform_.rotate = { 0,3.141592f,0.0f };
	if (rightHand_) {
		rightHand_->ConnectJoint(&rightArmJointWorldTransform_);
	}
	if (leftHand_) {
		leftHand_->ConnectJoint(&leftArmJointWorldTransform_);
	}
	if (head_) {
		head_->ConnectJoint(&headJointWorldTransform_);
	}
	rightArmJointWorldTransform_.transform_.translate =Ease::Easing(Ease::EaseName::Lerp, rightArmJointWorldTransform_.transform_.translate,rightHandRootPos_,0.05f);
	leftArmJointWorldTransform_.transform_.translate = Ease::Easing(Ease::EaseName::Lerp, leftArmJointWorldTransform_.transform_.translate, leftHandRootPos_, 0.05f);
	headJointWorldTransform_.transform_.translate = Ease::Easing(Ease::EaseName::Lerp, headJointWorldTransform_.transform_.translate, HeadInitPos_, 0.05f);
	if (countUp_ == 60) {
		if (executeAction_ == 1) {
			if (rightHand_ || leftHand_) {
				float random = RandomEngine::GetRandom(0.0f,1.0f);
				if (random < 0.5f) {
					state_ = std::bind(&Boss::RightStampAttack, this);
				}
				else {
					state_ = std::bind(&Boss::LeftStampAttack, this);
				}

			}
			else{
				state_ = std::bind(&Boss::HeadButtAttack, this);
			}
		}
		else {
			if (rightHand_ || leftHand_) {
				float random = RandomEngine::GetRandom(0.0f, 1.0f);
				if (random < 0.5f) {
					state_ = std::bind(&Boss::RightRoundAttack, this);
				}
				else {
					state_ = std::bind(&Boss::LeftRoundAttack, this);
				}

			}
			else {
				state_ = std::bind(&Boss::Summon, this);
			}
		}
		//state_ = std::bind(&Boss::HeadButtAttack, this);
		executeAction_ *= -1;
		countUp_ = 0;
		return;
	}

	if (!rightHand_ && !leftHand_) {//両手ともなかったら
		//CreateHand();
	}

	countUp_++;
}

void Boss::RightStampAttack() {
	if (rightHand_) {
		if (countUp_ == 0) {
			rightHand_->ConnectJoint(nullptr);
			rightHand_->Stamp();
		}
		countUp_=1;
	}
	else if (leftHand_) {
		state_ = std::bind(&Boss::LeftStampAttack, this);
	}
	else {
		state_ = std::bind(&Boss::Root, this);
	}
}

void Boss::LeftStampAttack() {
	if (leftHand_) {
		if (countUp_ == 0) {
			leftHand_->ConnectJoint(nullptr);
			leftHand_->Stamp();
		}
		countUp_ = 1;
	}
	else if(rightHand_){
		state_ = std::bind(&Boss::RightStampAttack, this);
	}
	else {
		state_ = std::bind(&Boss::Root, this);
	}
}

void Boss::HeadButtAttack() {
	if (head_) {
		if (countUp_ <= headButtMoveLength_) {
			float t = float(countUp_) / float(headButtMoveLength_);
			headJointWorldTransform_.transform_.translate = Ease::Easing(Ease::EaseName::Lerp, HeadInitPos_, HeadAttackPos_, t);
			RotateToPlayer();
			//ChacePlayerY();
		}
		if (countUp_ == headButtMoveLength_) {
			head_->AttackCall();
		}
		countUp_++;
	}

}

void Boss::RightRoundAttack() {
	if (rightHand_) {
		if (countUp_ <= kRightHandRoundMoveLength_) {
			float t = float(countUp_) / float(kRightHandRoundMoveLength_);
			Vector3 targetPos = rightHandRoundPos_;
			//プレイヤーのボス座標系でのローカル位置を取ってくる		
			Vector3 targetRocalPos_ = Matrix4x4::Transform(target_->GetWorldTransformAdress()->GetWorldPosition(),Matrix4x4::Inverse(worldTransform_.worldMatrix_));
			targetPos.z = targetRocalPos_.z;
			rightArmJointWorldTransform_.transform_.translate = Ease::Easing(Ease::EaseName::EaseOutQuad,rightHandRootPos_,targetPos,t);
		}
		if (countUp_ == kRightHandRoundMoveLength_) {
			rightHand_->Round();
		}
		countUp_++;
	}
	else if (leftHand_) {
		state_ = std::bind(&Boss::LeftRoundAttack, this);
	}
	else {
		state_ = std::bind(&Boss::Root, this);
	}
}

void Boss::LeftRoundAttack() {
	if (leftHand_) {
		if (countUp_ <= kRightHandRoundMoveLength_) {
			float t = float(countUp_) / float(kRightHandRoundMoveLength_);
			Vector3 targetPos = leftHandRoundPos_;
			//プレイヤーのボス座標系でのローカル位置を取ってくる		
			Vector3 targetRocalPos_ = Matrix4x4::Transform(target_->GetWorldTransformAdress()->GetWorldPosition(), Matrix4x4::Inverse(worldTransform_.worldMatrix_));
			targetPos.z = targetRocalPos_.z;
			leftArmJointWorldTransform_.transform_.translate = Ease::Easing(Ease::EaseName::EaseOutQuad, leftHandRootPos_, targetPos, t);
		}
		if (countUp_ == kRightHandRoundMoveLength_) {
			leftHand_->Round();
		}
		countUp_++;
	}
	else if (rightHand_) {
		state_ = std::bind(&Boss::RightRoundAttack, this);
	}
	else {
		state_ = std::bind(&Boss::Root, this);
	}
}

void Boss::Damage() {
		if (countUp_ <= damageAnimationlength_) {
			worldTransform_.transform_.translate = Ease::Easing(Ease::EaseName::Lerp, worldTransform_.transform_.translate, oridinSpownPos_, 0.05f);
		}
		if (countUp_ == damageAnimationlength_) {
			state_ = std::bind(&Boss::Spawn, this);
			countUp_ = 0;
			return;
		}
		countUp_++;
}

void Boss::Dead() {
	
	if (countUp_ <= damageAnimationlength_) {
		worldTransform_.transform_.translate = Ease::Easing(Ease::EaseName::Lerp, worldTransform_.transform_.translate, oridinSpownPos_, 0.05f);
	}
	if (countUp_ == damageAnimationlength_) {
		isDead_ = true;
		countUp_ = damageAnimationlength_;
		return;
	}
	countUp_++;
}

void Boss::Summon() {
	if (countUp_ == 0) {
		head_->Summon();
		//敵召喚する
		if (head_->GetHp() == 3) {
			SummonPhaseOne();
		}
		else if (head_->GetHp() == 2) {
			SummonPhaseTwo();
		}
		else {
			SummonPhaseThree();
		}
	}
	if (countUp_ == summonAnimationLength_) {
		head_->SummonEnd();
		state_ = std::bind(&Boss::Root, this);
		countUp_ = 0;
		return;
	}
	countUp_++;
}

void Boss::SummonPhaseOne() {
	EnemyData data;

	//wave1

	data.className = "Enemy";
	data.spownFrame = 180;
	data.position = { -8.0f,8.0f,0.0f };
	data.velocity = { 0,0,0 };
	enemyManager_->AddEnemy(data);

	data.className = "Enemy";
	data.spownFrame = 180;
	data.position = { 8.0f,8.0f,0.0f };
	data.velocity = { 0,0,0 };
	enemyManager_->AddEnemy(data);
}

void Boss::SummonPhaseTwo() {

}

void Boss::SummonPhaseThree() {

}

void Boss::Spawn() {
	if (countUp_ == 0) {
		CreateHand();
		worldTransform_.transform_.rotate = { 0,3.141592f,0.0f };
		rightArmJointWorldTransform_.transform_.translate = rightHandRootPos_;
		leftArmJointWorldTransform_.transform_.translate = leftHandRootPos_;
		headJointWorldTransform_.transform_.translate = HeadInitPos_;
	}

	if (countUp_ <= spawnAnimationLength_) {
		float t = float(countUp_) / float(spawnAnimationLength_);
		worldTransform_.transform_.translate = Ease::Easing(Ease::EaseName::EaseOutBack, oridinSpownPos_,oridinRootPos_, t);
	}
	if (countUp_ == spawnAnimationLength_) {
		state_ = std::bind(&Boss::Root, this);
		countUp_ = 0;
		return;
	}
	countUp_++;
}

void Boss::CreateHand() {
	LevelData::ObjectData data;

	IObject* pointer = nullptr;

	data = Hand::HandCreate(1);
	pointer = objectManager_->AddObject(data);
	static_cast<Hand*>(pointer)->SetPlayer(target_);
	static_cast<Hand*>(pointer)->SetParent(this);
	rightHand_ = static_cast<Hand*>(pointer);
	rightArmJointWorldTransform_.transform_.translate = rightHandInitPos_;
	rightArmJointWorldTransform_.UpdateMatrix();
	rightHand_->ConnectJoint(&rightArmJointWorldTransform_);
	
	data = Hand::HandCreate(-1);
	pointer = objectManager_->AddObject(data);
	static_cast<Hand*>(pointer)->SetPlayer(target_);
	static_cast<Hand*>(pointer)->SetParent(this);
	leftHand_ = static_cast<Hand*>(pointer);
	leftArmJointWorldTransform_.transform_.translate = leftHandInitPos_;
	leftArmJointWorldTransform_.UpdateMatrix();
	leftHand_->ConnectJoint(&leftArmJointWorldTransform_);
	
}

void Boss::CreateHead() {
	LevelData::ObjectData data;

	IObject* pointer = nullptr;

	data = Head::HeadCreate();
	pointer = objectManager_->AddObject(data);
	static_cast<Head*>(pointer)->SetPlayer(target_);
	static_cast<Head*>(pointer)->SetParent(this);
	head_ = static_cast<Head*>(pointer);
	headJointWorldTransform_.transform_.translate = HeadInitPos_;
	headJointWorldTransform_.UpdateMatrix();
	head_->ConnectJoint(&headJointWorldTransform_);
}

void Boss::EndAttack() {
	state_ = std::bind(&Boss::Root, this);
	if (rightHand_) {
		rightArmJointWorldTransform_.transform_.translate = Matrix4x4::Transform(rightHand_->GetWorldTransformAdress()->GetWorldPosition(), Matrix4x4::Inverse(worldTransform_.worldMatrix_));
	}
	if (leftHand_) {
		leftArmJointWorldTransform_.transform_.translate = Matrix4x4::Transform(leftHand_->GetWorldTransformAdress()->GetWorldPosition(), Matrix4x4::Inverse(worldTransform_.worldMatrix_));
	}
	countUp_ = 0;
}

void Boss::EndHeadAttack() {
	state_ = std::bind(&Boss::Root, this);
	worldTransform_.usedDirection_ = false;
	countUp_ = 0;
}

void Boss::DeathRightHand() {
	state_ = std::bind(&Boss::Root, this);
	//rightHand_->ConnectJoint(nullptr);
	rightHand_ = nullptr;
	countUp_ = 0;
}

void Boss::DeathLeftHand() {
	state_ = std::bind(&Boss::Root, this);
	//leftHand_->ConnectJoint(nullptr);
	leftHand_ = nullptr;
	countUp_ = 0;
}

void Boss::DamageHead() {
	state_ = std::bind(&Boss::Damage, this);
	countUp_ = 0;
}

void Boss::DeathHead() {
	state_ = std::bind(&Boss::Dead, this);
	countUp_ = 0;
	head_ = nullptr;
}

void Boss::RotateToPlayer() {
	Vector3 from = worldTransform_.GetWorldPosition();
	Vector3 to = target_->GetWorldTransformAdress()->GetWorldPosition();
	from.y = 0;
	to.y = 0;
	//プレイヤーの方を向く
	Matrix4x4 rotate = Matrix4x4::DirectionToDirection({ 0.0f,0.0f,1.0f }, to - from);
	worldTransform_.direction_ = Matrix4x4::TransformNormal({ 0.0f,0.0f,1.0f }, rotate);
	worldTransform_.usedDirection_ = true;
	worldTransform_.UpdateMatrix();
}

void Boss::ChacePlayerY() {
	float playerY = target_->GetWorldTransformAdress()->GetWorldPosition().y;
	float target = Block::kLowHight + 2.0f;
	if (playerY > Block::kFloatHight) {
		target += Block::kFloatHight;
	}
	moveTargetY_ = target;
	moveFromY_ = worldTransform_.transform_.translate.y;
	//worldTransform_.transform_.translate.y = Ease::Easing(Ease::EaseName::Lerp, worldTransform_.transform_.translate.y, target,0.05f);
}