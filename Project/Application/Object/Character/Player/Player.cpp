#include "Player.h"
#include "../../../../Engine/Collision/Extrusion.h"

#include "../../Obstacle/BaseObstacle.h"
#include "../../../../Engine/3D/ModelDraw.h"
#include "../../../../Engine/Physics/Gravity.h"

LevelData::MeshData Player::PlayerCreate()
{

	LevelData::MeshData data;

	// 名前
	data.name = "Player";
	// トランスフォーム
	data.transform = {
		0.3f,0.3f,0.3f,
		0.0f,0.0f,0.0f,
		0.0f,2.0f,0.0f
	};

	// ファイルの名前
	data.flieName = "PlayerMove.gltf";
	// ディレクトリパス
	data.directoryPath = "Resources/Model/Player/";
	// クラスの名前
	data.className = "Player";
	// 親の名前
	data.parentName = "";

	// コライダー(一時的なもの、親部分はヌルにしとく)
	OBB obb;
	obb.Initialize({ 0.0f,0.0f,0.0f }, Matrix4x4::MakeIdentity4x4(), { 1.0f,1.0f,1.0f }, static_cast<Null*>(nullptr));
	data.collider = obb;

	return data;

}

void Player::Initialize(LevelData::MeshData* data)
{


	MeshObject::Initialize(data);

	//衝突属性(自分)
	collisionAttribute_ = 0x00000001;

	// 衝突マスク(相手)
	collisionMask_ = 0xfffffffe;

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

	// コマンド
	playerCommand_ = PlayerCommand::GetInstance();
	playerCommand_->Initialize();

	// コマンドを受け取るか
	receiveCommand_ = true;

	// ステート
	StateInitialize();

	// パーツ
	PartInitialize();

	// hp
	hp_ = 3;
	initHp_ = 3;

	isDead_ = false;

	prePosition_ = worldTransform_.GetWorldPosition();

	// 初期設定
	material_->SetEnableLighting(BlinnPhongReflection);

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

}

void Player::Update()
{

	MeshObject::Update();

	if (receiveCommand_) {
		nextStateNo_ = playerCommand_->Command();
	}

	// ステート
	StateUpdate();

	// アニメーション
	AnimationUpdate();

	localMatrixManager_->SetNodeLocalMatrix(animation_.AnimationUpdate());
	
	localMatrixManager_->Map();

	// 重力
	velocity_ += Gravity::Execute();
	worldTransform_.transform_.translate += velocity_; 

	worldTransform_.UpdateMatrix();

	// コライダー
	ColliderUpdate();

	// 速度保存
	SaveVelocityUpdate();

}

void Player::Draw(BaseCamera& camera)
{

	ModelDraw::AnimObjectDesc desc;
	desc.camera = &camera;
	desc.localMatrixManager = localMatrixManager_.get();
	desc.material = material_.get();
	desc.model = model_;
	desc.worldTransform = &worldTransform_;
	ModelDraw::AnimObjectDraw(desc);

}

void Player::ImGuiDraw()
{
}

void Player::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	if (std::holds_alternative<BaseObstacle*>(colliderPartner)) {
		OnCollisionObstacle(colliderPartner, collisionData);
	}

}

void Player::ParticleDraw(BaseCamera& camera)
{
}

void Player::StateInitialize()
{

	// ステートファクトリー
	playerStateFactory_ = PlayerStateFactory::GetInstance();

	// ステート
	playerState_.reset(playerStateFactory_->CreatePlayerState(kPlayerStateRoot)); // 最初のステート
	playerState_->Initialize();

	// ステート番号
	currentStateNo_ = PlayerState::kPlayerStateRoot; // 最初のステート
	prevStateNo_ = PlayerState::kPlayerStateRoot; // 最初のステート
	nextStateNo_ = PlayerState::kPlayerStateRoot; // 最初のステート
	playerState_->SetPlayer(this); // プレイヤーセット

}

void Player::StateUpdate()
{

	// ステートのチェック
	prevStateNo_ = currentStateNo_;
	if (receiveCommand_) {
		currentStateNo_ = nextStateNo_;
	}
	else {
		currentStateNo_ = playerState_->GetPlaryerStateNo();
	}

	// ステートが変わったか
	if (prevStateNo_ != currentStateNo_) {
		//ステート変更（初期化）
		playerState_.reset(playerStateFactory_->CreatePlayerState(currentStateNo_));
		playerState_->Initialize();
	}

	// ステート更新
	playerState_->Update();

}

void Player::PartInitialize()
{

	// 現在のモーション番号
	currentMotionNo_ = PlayerMotionIndex::kPlayerMotionWait;

	// 前のモーション番号
	prevMotionNo_ = PlayerMotionIndex::kPlayerMotionWait;

	// 待ちアニメーション
	animation_.StartAnimation(kPlayerMotionWait, true);

}

void Player::ColliderUpdate()
{

	OBB obb = std::get<OBB>(*collider_.get());

	obb.center_ = worldTransform_.GetWorldPosition();
	obb.SetOtientatuons(worldTransform_.rotateMatrix_);

	ColliderShape* colliderShape = new ColliderShape();

	*colliderShape = obb;

	collider_->swap(*colliderShape);

}

void Player::AnimationUpdate()
{

	prevMotionNo_ = currentMotionNo_;
	currentMotionNo_ = playerState_->GetPlaryerMotionNo();

	if (currentMotionNo_ != prevMotionNo_) {
		animation_.StopAnimation(prevMotionNo_);
		animation_.StartAnimation(currentMotionNo_, true);
	}

}

void Player::OnCollisionObstacle(ColliderParentObject colliderPartner, const CollisionData& collisionData)
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
