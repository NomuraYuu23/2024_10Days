#include "Player.h"
#include "../../../../Engine/Collision/Extrusion.h"

#include "../../Obstacle/BaseObstacle.h"
#include "../Enemy/Enemy.h"
#include "../Enemy/FlyEnemy.h"
#include "../Enemy/Bullet.h"
#include "../Enemy/Boss/Hand.h"
#include "../Enemy/Boss/Head.h"
#include "../../../../Engine/3D/ModelDraw.h"
#include "../../../../Engine/Physics/Gravity.h"
#include "../../../../externals/imgui/imgui.h"
#include "../../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../../../Engine/Math/Ease.h"

LevelData::MeshData Player::PlayerCreate()
{

	LevelData::MeshData data;

	// 名前
	data.name = "Player";
	// トランスフォーム
	data.transform = {
		1.0f,1.0f,1.0f,
		0.0f,0.0f,0.0f,
		0.0f,2.0f,0.0f
	};

	// ファイルの名前
	data.flieName = "Player.gltf";
	// ディレクトリパス
	data.directoryPath = "Resources/Model/Player/";
	// クラスの名前
	data.className = "Player";
	// 親の名前
	data.parentName = "";

	// コライダー(一時的なもの、親部分はヌルにしとく)
	OBB obb;
	obb.Initialize({ 0.0f,0.0f,0.0f }, Matrix4x4::MakeIdentity4x4(), { 1.0f,2.5f,1.0f }, static_cast<Null*>(nullptr));
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
	playerCommand_->Initialize(this);

	// コマンドを受け取るか
	receiveCommand_ = true;

	changeStatedirectly = false;

	// ステート
	StateInitialize();

	// パーツ
	PartInitialize();

	// hp
	initHp_ = 3;

	runningSpeed_ = 0.3f;

	isDead_ = false;

	// ジャンプ初速
	jumpInitialSpeed_ = 1.4f;

	// ジャンプチェックポイント
	jumpCheckpointFrame_ = 8;

	// 小ジャンプ初速
	smallJumpInitialSpeed_ = 1.0f;

	// 滞空時倍率
	airborneMultiplier_ = 0.5f;

	// 滞空フラグ
	airborneCheck_ = false;

	//
	fallingPosition_ = { 1000.0f,1000.0f,1000.0f };

	fallSearchSpeedCorrection_ = 3.0f;

	knockbackDirection_ = {0.0f,0.0f,1.0f};

	receiveDamage_ = false;

	// 無敵
	isInvincible_ = false;

	// 無敵時間
	invincibilityTime_ = 2.0f;

	// 無敵経過時間
	invincibilityElapsedTime_ = invincibilityTime_;

	// 落下位置
	fallingPositionY_ = -10.0f;

	// リスポーン位置
	respawnPosition_ = {0.01f,35.0f,0.0f};

	prePosition_ = worldTransform_.GetWorldPosition();

	isPreGame_ = true;

	moveSoundSecond_ = false;

	// 初期設定
	material_->SetEnableLighting(BlinnPhongReflection);

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	RegistrationGlobalVariables();
	ApplyGlobalVariables();
	
	hp_ = initHp_;

	runDustParticle_ = std::make_unique<RunDustParticle>();

	runDustParticle_->Initialize(
		dxCommon->GetDevice(),
		dxCommon->GetCommadListLoad(),
		GraphicsPipelineState::sRootSignature[GraphicsPipelineState::kPipelineStateIndexGPUParticle].Get(),
		GraphicsPipelineState::sPipelineState[GraphicsPipelineState::kPipelineStateIndexGPUParticle].Get());

	EmitterCS emitter;
	emitter.count = 10;
	emitter.frequency = 0.1f;
	emitter.frequencyTime = 0.0f;
	emitter.translate = worldTransform_.GetWorldPosition();
	emitter.radius = 1.0f;
	emitter.emit = 0;

	runDustParticle_->SetEmitter(emitter);

}

void Player::Update()
{

#ifdef _DEMO
	ApplyGlobalVariables();

	if (Input::GetInstance()->TriggerKey(DIK_P)) {
		worldTransform_.transform_.translate = respawnPosition_;
	}

#endif // _DEMO

	if (isGameOver_) {
		return;
	}

	MeshObject::Update();

	if (receiveCommand_) {
		nextStateNo_ = playerCommand_->Command();
	}

	// 滞空フラグ
	airborneCheck_ = false;

	receiveDamage_ = false;

	// ステート
	StateUpdate();

	// アニメーション
	AnimationUpdate();

	localMatrixManager_->SetNodeLocalMatrix(animation_.AnimationUpdate());
	
	localMatrixManager_->Map();

	// 歩きサウンド再生
	if (currentMotionNo_ == kPlayerMotionRun && velocity_.y == 0.0f) {
		if (!moveSoundSecond_) {
			if (animation_.GetAnimationDatas()->at(kPlayerMotionRun).timer - kDeltaTime_ <= 0.0f) {
				audioManager_->PlayWave(kGameMoveSE);
				moveSoundSecond_ = true;
			}
		}
		else {
			if (animation_.GetAnimationDatas()->at(kPlayerMotionRun).timer - kDeltaTime_ >= 0.25f) {
				audioManager_->PlayWave(kGameMoveSE);
				moveSoundSecond_ = false;
			}
		}
	}
	else {
		moveSoundSecond_ = false;
	}


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

	// 砂ぼこり
	if (currentMotionNo_ == kPlayerMotionRun && velocity_.y == Gravity::Execute().y) {
		EmitterCS emitter;
		emitter.count = 2;
		emitter.frequency = 0.1f;
		emitter.frequencyTime = 0.0f;
		emitter.translate = worldTransform_.GetWorldPosition();
		emitter.radius = 1.0f;
		emitter.emit = 0;

		runDustParticle_->SetEmitter(emitter, false);
		runDustParticle_->Update();
	}
	else {
		EmitterCS emitter;
		emitter.count = 2;
		emitter.frequency = 0.1f;
		emitter.frequencyTime = 0.0f;
		emitter.translate = worldTransform_.GetWorldPosition();
		emitter.radius = 1.0f;
		emitter.emit = 0;

		runDustParticle_->SetEmitter(emitter, true);
		runDustParticle_->Update();
	}

	// 落下とリスポーン
	FallAndRespawn();

	// 無敵処理
	InvincibleUpdate();

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

	ImGui::Begin("Player");
	if (ImGui::Button("PositionReset")) {
		worldTransform_.transform_.translate = { 0.0f,20.0f,0.0f };
	}
	ImGui::End();


}

void Player::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	if (std::holds_alternative<Block*>(colliderPartner)) {
		lastToutchBlock_ = std::get<Block*>(colliderPartner);
		OnCollisionObstacle(colliderPartner, collisionData);
	}
	else if (std::holds_alternative<Enemy*>(colliderPartner)) {
		// 死亡アニメーションに入ってない
		if (!std::get<Enemy*>(colliderPartner)->GetIsPlayDeathAnimation_() && !isInvincible_) {
			OnCollisionDamage(std::get<Enemy*>(colliderPartner)->GetWorldTransformAdress()->GetWorldPosition());
		}
	}
	else if (std::holds_alternative<FlyEnemy*>(colliderPartner) && !isInvincible_) {
		// 死亡アニメーションに入ってない
		if (!std::get<FlyEnemy*>(colliderPartner)->GetIsPlayDeathAnimation_()) {
			OnCollisionDamage(std::get<FlyEnemy*>(colliderPartner)->GetWorldTransformAdress()->GetWorldPosition());
		}
	}
	else if (std::holds_alternative<Bullet*>(colliderPartner) && !isInvincible_) {
		OnCollisionDamage(std::get<Bullet*>(colliderPartner)->GetWorldTransformAdress()->GetWorldPosition());
	}
	else if (std::holds_alternative<Hand*>(colliderPartner) && !isInvincible_) {
		//攻撃判定があるかどうか
		if (std::get<Hand*>(colliderPartner)->IsAttack()) {
			OnCollisionDamage(std::get<Hand*>(colliderPartner)->GetWorldTransformAdress()->GetWorldPosition());
		}
	}
	else if (std::holds_alternative<Head*>(colliderPartner) && !isInvincible_) {
		//攻撃判定があるかどうか
		if (std::get<Head*>(colliderPartner)->IsAttack()) {
			OnCollisionDamage(std::get<Head*>(colliderPartner)->GetWorldTransformAdress()->GetWorldPosition());
		}
	}

}

void Player::ParticleDraw(BaseCamera& camera)
{

	runDustParticle_->Draw(DirectXCommon::GetInstance()->GetCommadList(), camera);

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
	if (receiveCommand_ || changeStatedirectly) {
		currentStateNo_ = nextStateNo_;
	}

	else {
		currentStateNo_ = playerState_->GetPlaryerStateNo();
	}

	changeStatedirectly = false;

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

	float coliderAddY = 1.0f;

	obb.center_ = worldTransform_.GetWorldPosition();
	obb.center_.y += obb.size_.y / 2.0f + coliderAddY;
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

	// 下からたたいた時、滞空
	if (extrusion.y < 0.0f && velocity_.y > 0.0f && !airborneCheck_) {
		velocity_.y += Gravity::Execute().y * -airborneMultiplier_;
		// 滞空フラグ
		airborneCheck_ = true;
	}

	worldTransform_.transform_.translate += extrusion;
	worldTransform_.UpdateMatrix();
	// コライダー
	ColliderUpdate();
}

void Player::OnCollisionDamage(const Vector3& position)
{

	if (receiveDamage_ || currentStateNo_ == kPlayerStateKnockback || currentStateNo_ == kPlayerStateHeadDrop) {
		return;
	}
	
	// ノックバック処理
	Vector3 playerPos = worldTransform_.GetWorldPosition();
	playerPos.y = position.y;

	knockbackDirection_ = Vector3::Normalize(Vector3::Subtract(position,playerPos));
	nextStateNo_ = kPlayerStateKnockback;
	receiveCommand_ = false;
	changeStatedirectly = true;

	// HP処理
	Damage();

	camera_->ShakeStart(1.0f, 0.5f);

}

void Player::PositionLimit()
{

	worldTransform_.transform_.translate.x = std::clamp(worldTransform_.transform_.translate.x, Block::kMinRange_.x, Block::kMaxRange_.x);
	worldTransform_.transform_.translate.y = std::clamp(worldTransform_.transform_.translate.y, Block::kMinRange_.y, Block::kMaxRange_.y);
	worldTransform_.transform_.translate.z = std::clamp(worldTransform_.transform_.translate.z, Block::kMinRange_.z, Block::kMaxRange_.z);

}

void Player::InvincibleUpdate()
{

	// 無敵タイマー処理
	if (isInvincible_) {
		invincibilityElapsedTime_ += kDeltaTime_;
		if (invincibilityElapsedTime_ >= invincibilityTime_) {
			isInvincible_ = false;
			invincibilityElapsedTime_ = invincibilityTime_;
		}
	}

	// マテリアル変更
	Vector4 color = { 1.0f,1.0f,1.0f,1.0f };
	color.z = Ease::Easing(Ease::EaseName::EaseInQuad, 0.0f, 1.0f, invincibilityElapsedTime_ / invincibilityTime_);
	material_->SetColor(color);


}

void Player::Damage()
{

	audioManager_->PlayWave(kGameHitAttackSE);
	receiveDamage_ = true;

	if (isPreGame_) {
		return;
	}

	hp_--;
	if (hp_ <= 0) {
		hp_ = 0;
		// 死んだ判定
		isGameOver_ = true;
	}

	// 無敵
	isInvincible_ = true;
	invincibilityElapsedTime_ = 0.0f;

}

void Player::FallAndRespawn()
{

	// 落下確認
	if (worldTransform_.GetWorldPosition().y <= fallingPositionY_) {
		// ダメージ
		Damage();
		// リスポーン
		worldTransform_.transform_.translate = respawnPosition_;
	}


}

void Player::ApplyGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	
	initHp_ = static_cast<uint32_t>(globalVariables->GetIntValue(groupName, "initHp"));
	runningSpeed_ = globalVariables->GetFloatValue(groupName, "runningSpeed");
	jumpInitialSpeed_ = globalVariables->GetFloatValue(groupName, "jumpInitialSpeed");
	jumpCheckpointFrame_ = globalVariables->GetIntValue(groupName, "jumpCheckpointFrame");
	smallJumpInitialSpeed_ = globalVariables->GetFloatValue(groupName, "smallJumpInitialSpeed");
	airborneMultiplier_ = globalVariables->GetFloatValue(groupName, "airborneMultiplier");
	fallSearchSpeedCorrection_ = globalVariables->GetFloatValue(groupName, "fallSearchSpeedCorrection");

}

void Player::RegistrationGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";

	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "initHp", static_cast<int32_t>(initHp_));
	globalVariables->AddItem(groupName, "runningSpeed", runningSpeed_);
	globalVariables->AddItem(groupName, "jumpInitialSpeed", jumpInitialSpeed_);
	globalVariables->AddItem(groupName, "jumpCheckpointFrame", jumpCheckpointFrame_);
	globalVariables->AddItem(groupName, "smallJumpInitialSpeed", smallJumpInitialSpeed_);
	globalVariables->AddItem(groupName, "airborneMultiplier", airborneMultiplier_);
	globalVariables->AddItem(groupName, "fallSearchSpeedCorrection", fallSearchSpeedCorrection_);

}
