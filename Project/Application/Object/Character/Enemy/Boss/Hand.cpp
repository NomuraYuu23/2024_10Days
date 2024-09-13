#include "Hand.h"
#include "Boss.h"
#include "../../../../../Engine/Collision/Extrusion.h"

#include "../../../Obstacle/BaseObstacle.h"
#include "../../../../../Engine/3D/ModelDraw.h"
#include "../../../../../Engine/Physics/Gravity.h"
#include "../../../../../externals/imgui/imgui.h"
#include "../../../../../Engine/GlobalVariables/GlobalVariables.h"

#include "../../../Engine/Object/BaseObjectManager.h"

#include "../../../Engine/Math/Ease.h"

LevelData::MeshData Hand::HandCreate(int32_t direction)
{

	LevelData::MeshData data;
	static size_t id;
	// 名前
	data.name = "Hand" + std::to_string(id++);
	// トランスフォーム
	data.transform = {
		1.0f,1.0f,1.0f,
		0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f
	};

	// ファイルの名前
	data.flieName = "Hand.obj";
	// ディレクトリパス
	data.directoryPath = "Resources/Model/Boss/";
	// クラスの名前
	data.className = "Hand";
	// 親の名前
	data.parentName = "";

	//
	data.transform.rotate.x = float(direction);

	// コライダー(一時的なもの、親部分はヌルにしとく)
	OBB obb;
	obb.Initialize({ 0.0f,0.0f,0.0f }, Matrix4x4::MakeIdentity4x4(), { 2.5f,5.0f,2.0f }, static_cast<Null*>(nullptr));
	data.collider = obb;

	return data;

}

void Hand::Initialize(LevelData::MeshData* data)
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
	initHp_ = 1;

	isDead_ = false;

	direction_ = (worldTransform_.transform_.rotate.x);
	worldTransform_.transform_.rotate.x = 0;

	prePosition_ = worldTransform_.GetWorldPosition();

	// 初期設定
	material_->SetEnableLighting(BlinnPhongReflection);

	//RegistrationGlobalVariables();
	//ApplyGlobalVariables();

	hp_ = initHp_;

	//初期ステート
	state_ = std::bind(&Hand::Root, this);

	arrow_ = std::make_unique<AttackArrowObject>();
	LevelData::MeshData adata = AttackArrowObject::ArrowCreate();
	arrow_->Initialize(&adata);
}

void Hand::Update()
{

#ifdef _DEMO
	//ApplyGlobalVariables();


#endif // _DEBUG

	MeshObject::Update();
	
	isCollisionObstacle_ = false;
	isAttack_ = false;
	isDamageMovingBlock_ = false;
	material_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
	state_();
	if (isCollisionObstacle_ && !isAttack_) {
		material_->SetColor({0.2f,0.2f,0.2f,1.0f});
	}

	worldTransform_.UpdateMatrix();

	// コライダー
	ColliderUpdate();

	isCollision_ = false;
	arrow_->Update(worldTransform_.GetWorldPosition());
}

void Hand::Draw(BaseCamera& camera)
{

	MeshObject::Draw(camera);
	if (isCollisionObstacle_ && !isAttack_) {
		arrow_->Draw(camera);
	}
}

void Hand::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	if (std::holds_alternative<Block*>(colliderPartner)) {
		if (isCollisionObstacle_) {
			OnCollisionObstacle(colliderPartner, collisionData);
		}
		if (isCollisionObstacle_ || isAttack_){
			if (std::get<Block*>(colliderPartner)->GetIsAttack() || (std::get<Block*>(colliderPartner)->GetIsMoveNow() && isDamageMovingBlock_)) {
				Hit();
			}
		}
	}
	else if (std::holds_alternative<Player*>(colliderPartner)) {
		if ((isCollisionObstacle_ || isAttack_) && std::get<Player*>(colliderPartner)->GetCurrentStateNo() == PlayerState::kPlayerStateHeadDrop) {
			Hit();
		}
	}
}

void Hand::Hit() {
	hp_--;
	if (hp_ > 0) {
		state_ = std::bind(&Hand::Damage, this);
	}
	else {
		state_ = std::bind(&Hand::Dead, this);
		if (direction_ == 1.0f) {
			parent_->DeathRightHand();
		}
		else {
			parent_->DeathLeftHand();
		}
	}
	countUp_ = 0;
	velocity_ = { 0.0f,5.0f,0.0f };
	isCollisionObstacle_ = false;
	isAttack_ = false;
}

void Hand::ParticleDraw(BaseCamera& camera)
{

}


void Hand::ColliderUpdate()
{

	OBB obb = std::get<OBB>(*collider_.get());

	float coliderAddY = 1.0f;

	obb.center_ = worldTransform_.GetWorldPosition();
	Matrix4x4 rotate = worldTransform_.worldMatrix_;
	rotate.m[3][0] = 0;
	rotate.m[3][1] = 0;
	rotate.m[3][2] = 0;
	obb.SetOtientatuons(rotate);

	ColliderShape* colliderShape = new ColliderShape();

	*colliderShape = obb;

	collider_->swap(*colliderShape);

}


void Hand::OnCollisionObstacle(ColliderParentObject colliderPartner, const CollisionData& collisionData)
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
		velocity_.y = 0.0f;
	}

	worldTransform_.transform_.translate += extrusion;
	worldTransform_.UpdateMatrix();
	// コライダー
	ColliderUpdate();

	isCollision_ = true;
}

/*
void Hand::ApplyGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Hand";

	initHp_ = static_cast<uint32_t>(globalVariables->GetIntValue(groupName, "initHp"));
	runningSpeed_ = globalVariables->GetFloatValue(groupName, "runningSpeed");
	oridinalScale_ = globalVariables->GetVector3Value(groupName, "scale");
	shotFrame_ = static_cast<uint32_t>(globalVariables->GetIntValue(groupName, "shotFrame"));
	threewayRotate_ = globalVariables->GetFloatValue(groupName, "threewayRotate");

}

void Hand::RegistrationGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Hand";

	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "initHp", static_cast<int32_t>(initHp_));
	globalVariables->AddItem(groupName, "runningSpeed", runningSpeed_);
	globalVariables->AddItem(groupName, "scale", worldTransform_.transform_.scale);
	globalVariables->AddItem(groupName, "shotFrame", static_cast<int32_t>(shotFrame_));
	globalVariables->AddItem(groupName, "threewayRotate", threewayRotate_);
}
*/

void Hand::Root() {
	worldTransform_.transform_.translate = {0.0f,0.0f,0.0f};
	worldTransform_.transform_.rotate = { 0.0f,0.0f,0.0f };
	stampSE_ = false;
}

void Hand::StampStand() {

	worldTransform_.transform_.rotate = { 3.141592f * 0.5f ,0,0};
	velocity_ = {0,0,0};
	if (countUp_< stampChaseLength_) {
		Vector3 target = target_->GetWorldTransformAdress()->GetWorldPosition();
		target.y = 48.0f;
		worldTransform_.transform_.translate = Ease::Easing(Ease::EaseName::Lerp, worldTransform_.transform_.translate, target, 0.05f);
	}
	if (countUp_ == stampChaseLength_) {
		state_ = std::bind(&Hand::StampAttack, this);
		isCollision_ = false;
		isStampAttack_ = true;
		countUp_=0;
	}
	countUp_++;
}


void Hand::StampAttack() {
	isCollisionObstacle_ = true;
	if (isStampAttack_) {
		isAttack_ = true;
	}
	// 重力
	velocity_ += Gravity::Execute()*2.0f;
	// 速度制限
	//velocity_.y = std::fmaxf(velocity_.y, -1.0f);
	// 位置更新
	worldTransform_.transform_.translate += velocity_;
	if (isCollision_){
		if (!stampSE_) {
			stampSE_ = true;
			parent_->GetAudioManager()->PlayWave(kGameHeadDropSE);
		}
		isAttack_ = false;
		isStampAttack_ = false;
		if (countUp_ > stampStiffnessLength){//仮
			state_ = std::bind(&Hand::Root, this);
			parent_->EndAttack();
		}
		countUp_++;
	}
	if (worldTransform_.transform_.translate.y <= -100.0f) {
		state_ = std::bind(&Hand::Root, this);
		parent_->EndAttack();
	}
}

void Hand::RoundStand() {
	worldTransform_.transform_.rotate = { 3.141592f * 0.5f ,0,3.141592f * 0.5f * direction_ };
}

void Hand::RoundAttack() {
	//isCollisionObstacle_ = true;
	isAttack_ = true;
	//isDamageMovingBlock_ = true;
	worldTransform_.transform_.rotate = { 3.141592f * 0.5f ,0.0f,3.141592f * 0.5f * -direction_ };
	float t = float(countUp_) / float(kRoundAnimationLength_);
	worldTransform_.transform_.translate.x = Ease::Easing(Ease::EaseName::EaseInBack,0, -direction_* roundAttackWidth_, t);
	if (countUp_ > kRoundAnimationLength_) {//仮
		state_ = std::bind(&Hand::Root, this);
		parent_->EndAttack();
	}
	countUp_++;
}

void Hand::Damage() {
	
	//float t = float(countUp_) / float(damageAnimationLength);
	material_->SetColor({1.0f,0.2f,0.2f,1.0f});
	if (countUp_ < damageAnimationLength / 2) {
		//velocity_ += acceleration_;
		worldTransform_.transform_.translate += velocity_;
		velocity_ *= 0.8f;
	}
	if (countUp_ > damageAnimationLength) {
		material_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
		state_ = std::bind(&Hand::Root, this);
		parent_->EndAttack();
	}
	countUp_++;
}

void Hand::Dead() {

	//float t = float(countUp_) / float(damageAnimationLength);
	material_->SetColor({ 1.0f,0.2f,0.2f,1.0f });
	//velocity_ += acceleration_;
	worldTransform_.transform_.translate += velocity_;
	worldTransform_.transform_.rotate.x += 0.5f;
	worldTransform_.transform_.rotate.y += 0.5f;
	worldTransform_.transform_.rotate.z += 0.5f;
	velocity_ *= 0.9f;
	if (countUp_ > deathAnimationLength) {
		isDead_ = true;
	}
	countUp_++;
}

void Hand::Stamp() {
	state_ = std::bind(&Hand::StampStand, this);
	countUp_ = 0;
}

void Hand::Round() {
	state_ = std::bind(&Hand::RoundAttack , this);
	countUp_ = 0;
}

void Hand::ConnectJoint(WorldTransform* pointer) {
	if (pointer!= worldTransform_.parent_) {
		worldTransform_.transform_.translate = worldTransform_.GetWorldPosition();
		worldTransform_.SetParent(pointer);
		worldTransform_.UpdateMatrix();
	}
	//else {

	//}
}