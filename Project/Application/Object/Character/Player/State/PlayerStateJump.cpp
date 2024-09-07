#include "PlayerStateJump.h"
#include "../Player.h"
#include "../../../../../Engine/Math/Ease.h"
#include "../../../../../Engine/Math/DeltaTime.h"

void PlayerStateJump::Initialize()
{

	targetDirection_ = { 0.0f, 0.0f, 1.0f };

	targetAngleT_ = 0.1f;

	playerMotionNo_ = kPlayerMotionJump;

	playerStateNo_ = kPlayerStateJump;

	player_->SetVelocity(Vector3{0.0f, player_->GetJumpInitialSpeed() , 0.0f});

	// ジャンプしてからの時間
	jumpElapsedTime_ = 0.0f;

	// チェックポイント
	checkpointFlg_ = false;

	// アニメーションを止めるフラグ
	animStop_ = true;

}

void PlayerStateJump::Update()
{

	WorldTransform* worldTransform = player_->GetWorldTransformAdress();

	worldTransform->usedDirection_ = true;
	targetDirection_ = worldTransform->direction_;

	//移動
	if (input_->GetJoystickConnected()) {

		const float kThresholdRunning = 0.9f;

		// 移動量
		Vector3 move = { input_->GetLeftAnalogstick().x, 0.0f, -input_->GetLeftAnalogstick().y };
		if (Vector3::Length(move) > kThresholdRunning) {
			//ランニング
			Move(move, worldTransform, player_->GetRunningSpeed());
		}

		// 角度補間
		worldTransform->direction_ = Ease::Easing(Ease::EaseName::Lerp, worldTransform->direction_, targetDirection_, targetAngleT_);
	}

	player_->SetReceiveCommand(false);

	// 終了確認
	jumpElapsedTime_ += kDeltaTime_;

	if (!checkpointFlg_ && jumpElapsedTime_ > kDeltaTime_ * static_cast<float>(player_->GetJumpCheckpointFrame())) {
		checkpointFlg_ = true;
		if (input_->NoPushJoystick(JoystickButton::kJoystickButtonA)) {
			Vector3 velocityTmp = player_->GetVelocity();
			velocityTmp.y *= player_->GetSmallJumpMultiplier();
			player_->SetVelocity(velocityTmp);
		}
		else {
			animStop_ = false;
		}
	}

	if (player_->GetVelocity().y <= 0.0f) {
		
		// 高く飛んでいる && 下の足場がひくい位置にあるならドロップ

		// プレイヤーの位置は高いか
		if (worldTransform->GetWorldPosition().y < 40.0f) {
			playerStateNo_ = kPlayerStateFloating;
		}
		else {

			// ブロック情報
			BlockManager* blockManager = player_->GetBlockManager();
			std::vector<Block*>* blocks = blockManager->GetBlocks();
			Vector3 blockPos{};
			
			// プレイヤーの位置
			Vector3 playerPos = worldTransform->GetWorldPosition();
			// ブロックまでの距離
			Vector2 distanceToBlock{};
			// 範囲内距離
			float distance = Block::kSize_ + std::get<OBB>(*player_->GetCollider()).size_.x;
			// フラグ
			bool dropFlg = false;

			for (uint32_t i = 0; i < blockManager->GetBlockNum(); ++i) {

				blockPos = blocks->at(i)->GetWorldTransformAdress()->GetWorldPosition();
				distanceToBlock = { fabsf(playerPos.x - blockPos.x), fabsf(playerPos.z - blockPos.z) };

				// 範囲内確認
				if ((distance >= distanceToBlock.x && distance >= distanceToBlock.y) &&
					blockPos.y == -2.0f) {
					dropFlg = true;
					break;
				}

			}

			// フラグが立っていればドロップ
			if (dropFlg) {
				playerStateNo_ = kPlayerStateHeadDrop;
			}
			else {
				playerStateNo_ = kPlayerStateFloating;
			}

		}

	}

	// アニメーション確認
	Animation* animation = player_->GetAnimationAdress();

	if (animStop_) {
		double animTimer = 0.0f;
		animation->AnimationTimerFix(kPlayerMotionJump, animTimer);
	}

}

void PlayerStateJump::Move(Vector3& move, WorldTransform* worldTransform, float speed)
{

	BaseCamera* camera = player_->GetCamera();
	Vector3 velocity = { 0.0f, 0.0f, 0.0f };

	// 移動量に速さを反映
	move = Vector3::Multiply(speed, Vector3::Normalize(move));

	// カメラの角度から回転行列を計算する
	Matrix4x4 rotateMatrix = Matrix4x4::MakeRotateXYZMatrix(camera->GetRotate());

	// 移動ベクトルをカメラの角度だけ回転する
	move = Matrix4x4::TransformNormal(move, rotateMatrix);

	//　親がいれば
	if (worldTransform->parent_) {
		rotateMatrix = Matrix4x4::Inverse(worldTransform->parent_->rotateMatrix_);
		move = Matrix4x4::TransformNormal(move, rotateMatrix);
	}

	// 移動
	velocity.x = move.x;
	velocity.z = move.z;

	// 移動方向に見た目を合わせる(Y軸)
	// あたん
	targetDirection_.x = Vector3::Normalize(move).x;
	targetDirection_.z = Vector3::Normalize(move).z;

	worldTransform->transform_.translate = Vector3::Add(worldTransform->transform_.translate, velocity);

}
