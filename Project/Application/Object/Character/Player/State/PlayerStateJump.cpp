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

	// ジャンプしてからの時間
	jumpElapsedTime_ = 0.0f;

	// アニメーションを止めるフラグ
	animStop_ = true;

	// 踏み込み中か
	steppingIn_ = true;

	// ジャンプ移動倍率
	moveMagnification_ = 1.5f;

	player_->GetAudioManager()->PlayWave(kGamejumpSE);

}

void PlayerStateJump::Update()
{

	WorldTransform* worldTransform = player_->GetWorldTransformAdress();

	worldTransform->usedDirection_ = true;
	targetDirection_ = worldTransform->direction_;

	Animation* animation = player_->GetAnimationAdress();
	double animTimer = 0.0f;

	//移動
	if (!steppingIn_) {
		if (input_->GetJoystickConnected()) {

			const float kThresholdRunning = 0.9f;

			// 移動量
			Vector3 move = { input_->GetLeftAnalogstick().x, 0.0f, -input_->GetLeftAnalogstick().y };
			if (Vector3::Length(move) > kThresholdRunning) {
				//ランニング
				Move(move, worldTransform, player_->GetRunningSpeed() * moveMagnification_);
			}

			// 角度補間
			worldTransform->direction_ = Ease::Easing(Ease::EaseName::Lerp, worldTransform->direction_, targetDirection_, targetAngleT_);
		}
	}

	player_->SetReceiveCommand(false);

	// 終了確認
	jumpElapsedTime_ += kDeltaTime_;

	if (steppingIn_ && jumpElapsedTime_ > kDeltaTime_ * static_cast<float>(player_->GetJumpCheckpointFrame())) {

		steppingIn_ = false;
		if (input_->NoPushJoystick(JoystickButton::kJoystickButtonA)) {
			animStop_ = false;
			animTimer = static_cast<double>(kDeltaTime_) * player_->GetJumpCheckpointFrame();
			animation->AnimationTimerFix(kPlayerMotionJump, animTimer);
			player_->SetVelocity(Vector3{ 0.0f, player_->GetSmallJumpInitialSpeed(), 0.0f });
		}
		else {
			animStop_ = false;
			animTimer = static_cast<double>(kDeltaTime_) * player_->GetJumpCheckpointFrame();
			animation->AnimationTimerFix(kPlayerMotionJump, animTimer);
			player_->SetVelocity(Vector3{ 0.0f, player_->GetJumpInitialSpeed() , 0.0f });
		}
	}

	if (player_->GetVelocity().y <= 0.0f && !steppingIn_) {

		// 高く飛んでいる && 下の足場がひくい位置にあるならドロップ

		// プレイヤーの位置は高いか

		bool positionedHigh = (worldTransform->GetWorldPosition().y >= 40.0f);

		// ブロック情報
		BlockManager* blockManager = player_->GetBlockManager();
		std::vector<Block*>* blocks = blockManager->GetBlocks();
		Vector3 blockPos{};

		// プレイヤーの位置
		Vector3 playerPos = worldTransform->GetWorldPosition();
		
		/// 補正
		
		// 速度補正倍率
		float speedCorrection = player_->GetFallSearchSpeedCorrection();

		BaseCamera* camera = player_->GetCamera();
		Vector3 velocity = { 0.0f, 0.0f, 0.0f };

		// 移動量に速さを反映
		Vector3 move = Vector3::Normalize({ input_->GetLeftAnalogstick().x, 0.0f, -input_->GetLeftAnalogstick().y });

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

		Vector2 velocityDir = { velocity.x, velocity.z };
		velocityDir = Vector2::Normalize(velocityDir);

		playerPos.x += velocityDir.x * speedCorrection;
		playerPos.z += velocityDir.y * speedCorrection;

		/// 補正ここまで

		// ブロックまでの距離
		Vector2 distanceToBlock{};
		// 範囲内距離
		float distance = Block::kSize_ + std::get<OBB>(*player_->GetCollider()).size_.x;
		// フラグ
		bool dropFlg = false;

		for (uint32_t i = 0; i < blockManager->GetBlockNum(); ++i) {

			Block* block = blocks->at(i);

			blockPos = block->GetWorldTransformAdress()->GetWorldPosition();
			distanceToBlock = { fabsf(playerPos.x - blockPos.x), fabsf(playerPos.z - blockPos.z) };

			// 範囲内確認 高さ確認 上昇確認
			if ((distance >= Vector2::Length(distanceToBlock)) && (blockPos.y < playerPos.y) && !(block->GetIsMoveNow() && !block->GetHight()) ) {
				// ドロップになる
				if (((block->GetIsMoveNow() && block->GetHight()) || blockPos.y == -2.0f) 
					&& positionedHigh) {
					dropFlg = true;
				}
				else {
					dropFlg = false;
				}

				distance = Vector2::Length(distanceToBlock);
				
				Vector3 fallingPosition = { blockPos.x,  0.0f ,blockPos.z };

				player_->SetFallingPosition(fallingPosition);

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

	// アニメーション確認

	if (animStop_) {
		animTimer = 0.0f;
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
