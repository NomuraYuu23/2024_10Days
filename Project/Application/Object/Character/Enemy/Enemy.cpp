#include "Enemy.h"
#include "../../../../Engine/Collision/Extrusion.h"

#include "../../Obstacle/BaseObstacle.h"
#include "../../../../Engine/3D/ModelDraw.h"
#include "../../../../Engine/Physics/Gravity.h"
#include "../../../../externals/imgui/imgui.h"
#include "../../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../Obstacle/Block/Block.h"
LevelData::MeshData Enemy::EnemyCreate()
{

	LevelData::MeshData data;
	static size_t id;
	// 名前
	data.name = "Enemy" + std::to_string(id++);
	// トランスフォーム
	data.transform = {
		1.0f,1.0f,1.0f,
		0.0f,0.0f,0.0f,
		0.0f,2.0f,0.0f
	};

	// ファイルの名前
	data.flieName = "Spino.gltf";
	// ディレクトリパス
	data.directoryPath = "Resources/Model/Enemy/";
	// クラスの名前
	data.className = "Enemy";
	// 親の名前
	data.parentName = "";

	// コライダー(一時的なもの、親部分はヌルにしとく)
	OBB obb;
	obb.Initialize({ 0.0f,0.0f,0.0f }, Matrix4x4::MakeIdentity4x4(), { 1.0f,2.5f,1.0f }, static_cast<Null*>(nullptr));
	data.collider = obb;

	return data;

}

void Enemy::Initialize(LevelData::MeshData* data)
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

	//localMatrixManager_ = std::make_unique<LocalMatrixManager>();
	//localMatrixManager_->Initialize(model_->GetRootNode());

	/*animation_.Initialize(
		model_->GetNodeAnimationData(),
		localMatrixManager_->GetInitTransform(),
		localMatrixManager_->GetNodeNames());*/

	// パーツ
	PartInitialize();

	// hp
	initHp_ = 3;

	runningSpeed_ = 0.3f;

	isDead_ = false;

	prePosition_ = worldTransform_.GetWorldPosition();

	// 初期設定
	material_->SetEnableLighting(BlinnPhongReflection);

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	RegistrationGlobalVariables();
	ApplyGlobalVariables();

	hp_ = initHp_;

	//初期ステート
	state_ = std::bind(&Enemy::Rush, this);
}

void Enemy::Update()
{

#ifdef _DEMO
	ApplyGlobalVariables();


#endif // _DEBUG

	MeshObject::Update();

	CheckFloorConect();

	state_();

	// アニメーション
	AnimationUpdate();

	//localMatrixManager_->SetNodeLocalMatrix(animation_.AnimationUpdate());

	//localMatrixManager_->Map();

	// 重力
	velocity_ += Gravity::Execute();
	// 速度制限
	velocity_.y = std::fmaxf(velocity_.y, -1.0f);
	// 位置更新
	worldTransform_.transform_.translate += velocity_;
	// 位置制限
	PositionLimit();

	worldTransform_.UpdateMatrix();

	// コライダー
	ColliderUpdate();

	// 速度保存
	SaveVelocityUpdate();

}

void Enemy::Draw(BaseCamera& camera)
{
	MeshObject::Draw(camera);
	/*ModelDraw::AnimObjectDesc desc;
	desc.camera = &camera;
	desc.localMatrixManager = localMatrixManager_.get();
	desc.material = material_.get();
	desc.model = model_;
	desc.worldTransform = &worldTransform_;
	ModelDraw::AnimObjectDraw(desc);
	*/
}

void Enemy::ImGuiDraw()
{

}

void Enemy::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	if (std::holds_alternative<BaseObstacle*>(colliderPartner)) {
		OnCollisionObstacle(colliderPartner, collisionData);
	}

}

void Enemy::ParticleDraw(BaseCamera& camera)
{
}


void Enemy::PartInitialize()
{

	/*// 現在のモーション番号
	currentMotionNo_ = EnemyMotionIndex::kEnemyMotionWait;

	// 前のモーション番号
	prevMotionNo_ = EnemyMotionIndex::kEnemyMotionWait;

	// 待ちアニメーション
	animation_.StartAnimation(kEnemyMotionWait, true);
	*/
}

void Enemy::ColliderUpdate()
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

void Enemy::AnimationUpdate()
{
	/*
	prevMotionNo_ = currentMotionNo_;
	currentMotionNo_ = EnemyState_->GetPlaryerMotionNo();

	if (currentMotionNo_ != prevMotionNo_) {
		animation_.StopAnimation(prevMotionNo_);
		animation_.StartAnimation(currentMotionNo_, true);
	}
	*/
}

void Enemy::OnCollisionObstacle(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	BaseObstacle* obstacle = std::get<BaseObstacle*>(colliderPartner);

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
}

void Enemy::PositionLimit()
{

	Vector3 Max = { 18.0f,1000.0f, 18.0f };
	Vector3 Min = { -18.0f,-1000.0f, -18.0f };

	worldTransform_.transform_.translate.x = std::clamp(worldTransform_.transform_.translate.x, Min.x, Max.x);
	worldTransform_.transform_.translate.y = std::clamp(worldTransform_.transform_.translate.y, Min.y, Max.y);
	worldTransform_.transform_.translate.z = std::clamp(worldTransform_.transform_.translate.z, Min.z, Max.z);

}

void Enemy::ApplyGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Enemy";

	initHp_ = static_cast<uint32_t>(globalVariables->GetIntValue(groupName, "initHp"));
	runningSpeed_ = globalVariables->GetFloatValue(groupName, "runningSpeed");

}

void Enemy::RegistrationGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Enemy";

	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "initHp", static_cast<int32_t>(initHp_));
	globalVariables->AddItem(groupName, "runningSpeed", runningSpeed_);
}


void Enemy::Rush() {
	RotateToPlayer();
	Vector3 from = worldTransform_.GetWorldPosition();
	Vector3 to = target_->GetWorldTransformAdress()->GetWorldPosition();
	from.y = 0;
	to.y = 0;
	Vector3 move = Vector3::Normalize(to-from);

	move.y = 0;
	move *= runningSpeed_;
	worldTransform_.transform_.translate += move;
}


void Enemy::RushStart() {

}

void Enemy::Shot() {
	RotateToPlayer();
}

void Enemy::ShotStart() {

}

void Enemy::RotateToPlayer() {
	Vector3 from = worldTransform_.GetWorldPosition();
	Vector3 to = target_->GetWorldTransformAdress()->GetWorldPosition();
	from.y = 0;
	to.y = 0;
	//プレイヤーの方を向く
	Matrix4x4 rotate = Matrix4x4::DirectionToDirection({ 0.0f,0.0f,1.0f }, to - from);
	worldTransform_.direction_ = Matrix4x4::TransformNormal({ 0.0f,0.0f,1.0f }, rotate);
	worldTransform_.usedDirection_ = true;
}

void Enemy::CheckFloorConect() {
	bool hight = false;
	if (worldTransform_.GetWorldPosition().y > Block::kFloatHight) {
		hight = true;
	}
	Vector3 from = worldTransform_.GetWorldPosition();
	Vector3 to = target_->GetWorldTransformAdress()->GetWorldPosition();
	from.y = 0;
	to.y = 0;
	if (blockManager_->IsConnectRoad(from, to, hight)) {
		//突進に移行
		state_ = std::bind(&Enemy::Rush, this);
	}
	else {
		state_ = std::bind(&Enemy::Shot, this);
	}
}

