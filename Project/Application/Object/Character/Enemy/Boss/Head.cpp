#include "Head.h"
#include "Boss.h"
#include "../../../../../Engine/Collision/Extrusion.h"

#include "../../../Obstacle/BaseObstacle.h"
#include "../../../../../Engine/3D/ModelDraw.h"
#include "../../../../../Engine/Physics/Gravity.h"
#include "../../../../../externals/imgui/imgui.h"
#include "../../../../../Engine/GlobalVariables/GlobalVariables.h"

#include "../../../Engine/Object/BaseObjectManager.h"

#include "../../../Engine/Math/Ease.h"

LevelData::MeshData Head::HeadCreate()
{

	LevelData::MeshData data;
	static size_t id;
	// 名前
	data.name = "Head" + std::to_string(id++);
	// トランスフォーム
	data.transform = {
		1.0f,1.0f,1.0f,
		0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f
	};

	// ファイルの名前
	data.flieName = "Boss.gltf";
	// ディレクトリパス
	data.directoryPath = "Resources/Model/Boss/";
	// クラスの名前
	data.className = "Head";
	// 親の名前
	data.parentName = "";

	// コライダー(一時的なもの、親部分はヌルにしとく)
	OBB obb;
	obb.Initialize({ 0.0f,0.0f,0.0f }, Matrix4x4::MakeIdentity4x4(), { 1.0f,2.5f,1.0f }, static_cast<Null*>(nullptr));
	data.collider = obb;

	return data;

}

void Head::Initialize(LevelData::MeshData* data)
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

	localMatrixManager_ = std::make_unique<LocalMatrixManager>();
	localMatrixManager_->Initialize(model_->GetRootNode());

	animation_.Initialize(
		model_->GetNodeAnimationData(),
		localMatrixManager_->GetInitTransform(),
		localMatrixManager_->GetNodeNames());

	// パーツ
	PartInitialize();


	// hp
	initHp_ = 3;

	isDead_ = false;

	worldTransform_.transform_.rotate.x = 0;

	prePosition_ = worldTransform_.GetWorldPosition();

	// 初期設定
	material_->SetEnableLighting(BlinnPhongReflection);

	//RegistrationGlobalVariables();
	//ApplyGlobalVariables();

	hp_ = initHp_;

	//初期ステート
	state_ = std::bind(&Head::Root, this);

}

void Head::Update()
{

#ifdef _DEMO
	//ApplyGlobalVariables();


#endif // _DEBUG

	MeshObject::Update();

	isCollisionObstacle_ = false;
	isAttack_ = false;
	isDamageMovingBlock_ = false;
	state_();

	worldTransform_.UpdateMatrix();


	// アニメーション
	AnimationUpdate();

	localMatrixManager_->SetNodeLocalMatrix(animation_.AnimationUpdate());

	localMatrixManager_->Map();

	// コライダー
	ColliderUpdate();

	isCollision_ = false;
}

void Head::Draw(BaseCamera& camera)
{

	ModelDraw::AnimObjectDesc desc;
	desc.camera = &camera;
	desc.localMatrixManager = localMatrixManager_.get();
	desc.material = material_.get();
	desc.model = model_;
	desc.worldTransform = &worldTransform_;
	ModelDraw::AnimObjectDraw(desc);

}

void Head::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	if (std::holds_alternative<Block*>(colliderPartner)) {
		OnCollisionObstacle(colliderPartner, collisionData);
		if (isCollisionObstacle_) {
			if (std::get<Block*>(colliderPartner)->GetIsAttack() || (std::get<Block*>(colliderPartner)->GetIsMoveNow() && isDamageMovingBlock_)) {
				hp_--;
				if (hp_ > 0) {
					state_ = std::bind(&Head::Damage, this);
				}
				else {
					state_ = std::bind(&Head::Dead, this);
					//parent_->DeathLeftHead();
				}
				//isHitCoolTime_ = true;
				countUp_ = 0;
				if (std::get<Block*>(colliderPartner)->GetIsAttack()) {
					velocity_ = { 0.0f,5.0f,0.0f };
				}
				else {
					velocity_ = Vector3::Normalize(worldTransform_.GetWorldPosition() - std::get<Block*>(colliderPartner)->GetWorldTransformAdress()->GetWorldPosition()) * 5.0f;
				}
			}
		}
	}

}

void Head::ParticleDraw(BaseCamera& camera)
{

}


void Head::ColliderUpdate()
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


void Head::OnCollisionObstacle(ColliderParentObject colliderPartner, const CollisionData& collisionData)
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
void Head::ApplyGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Head";

	initHp_ = static_cast<uint32_t>(globalVariables->GetIntValue(groupName, "initHp"));
	runningSpeed_ = globalVariables->GetFloatValue(groupName, "runningSpeed");
	oridinalScale_ = globalVariables->GetVector3Value(groupName, "scale");
	shotFrame_ = static_cast<uint32_t>(globalVariables->GetIntValue(groupName, "shotFrame"));
	threewayRotate_ = globalVariables->GetFloatValue(groupName, "threewayRotate");

}

void Head::RegistrationGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Head";

	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "initHp", static_cast<int32_t>(initHp_));
	globalVariables->AddItem(groupName, "runningSpeed", runningSpeed_);
	globalVariables->AddItem(groupName, "scale", worldTransform_.transform_.scale);
	globalVariables->AddItem(groupName, "shotFrame", static_cast<int32_t>(shotFrame_));
	globalVariables->AddItem(groupName, "threewayRotate", threewayRotate_);
}
*/

void Head::Root() {
	worldTransform_.transform_.translate = { 0.0f,0.0f,0.0f };
	worldTransform_.transform_.rotate = { 0.0f,0.0f,0.0f };
}

void Head::Damage() {

	//float t = float(countUp_) / float(damageAnimationLength);
	material_->SetColor({ 1.0f,0.2f,0.2f,1.0f });
	if (countUp_ < damageAnimationLength / 2) {
		//velocity_ += acceleration_;
		worldTransform_.transform_.translate += velocity_;
		velocity_ *= 0.8f;
	}
	if (countUp_ > damageAnimationLength) {
		material_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
		state_ = std::bind(&Head::Root, this);
		parent_->EndAttack();
	}
	countUp_++;
}

void Head::Dead() {

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


void Head::ConnectJoint(WorldTransform* pointer) {
	if (pointer != worldTransform_.parent_) {
		worldTransform_.transform_.translate = worldTransform_.GetWorldPosition();
		worldTransform_.SetParent(pointer);
		worldTransform_.UpdateMatrix();
	}
	//else {

	//}
}

void Head::AnimationUpdate()
{

	if (currentMotionNo_ != prevMotionNo_) {
		animation_.StopAnimation(prevMotionNo_);
		animation_.StartAnimation(currentMotionNo_, true);
	}

	prevMotionNo_ = currentMotionNo_;

}

void Head::PartInitialize()
{

	// 現在のモーション番号
	currentMotionNo_ = HeadMotionIndex::kHeadMotionNormal;

	// 前のモーション番号
	prevMotionNo_ = HeadMotionIndex::kHeadMotionNormal;

	// 待ちアニメーション
	animation_.StartAnimation(kHeadMotionNormal, true);

}