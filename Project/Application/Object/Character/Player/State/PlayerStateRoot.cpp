#include "PlayerStateRoot.h"
#include "../Player.h"
#include "../../../../../Engine/Math/Ease.h"

void PlayerStateRoot::Initialize()
{

	targetDirection_ = { 0.0f, 0.0f, 1.0f };

	targetAngleT_ = 0.1f;

	playerMotionNo_ = kPlayerMotionWait;

	playerStateNo_ = kPlayerStateRoot;

}

void PlayerStateRoot::Update()
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
			playerMotionNo_ = kPlayerMotionRun;
		}
		else {
			playerMotionNo_ = kPlayerMotionWait;
		}

		// 角度補間
		worldTransform->direction_ = Ease::Easing(Ease::EaseName::Lerp, worldTransform->direction_, targetDirection_, targetAngleT_);
	}
	else {

		// 移動量
		Vector3 move = { 0.0f, 0.0f, 0.0f };

		if (input_->PushKey(DIK_W) || input_->PushKey(DIK_UP)) {
			move.z += 1.0f;
		}

		if (input_->PushKey(DIK_S) || input_->PushKey(DIK_DOWN)) {
			move.z -= 1.0f;
		}

		if (input_->PushKey(DIK_D) || input_->PushKey(DIK_RIGHT)) {
			move.x += 1.0f;
		}

		if (input_->PushKey(DIK_A) || input_->PushKey(DIK_LEFT)) {
			move.x -= 1.0f;
		}

		move = Vector3::Normalize(move);

		if (Vector3::Length(move) != 0.0f) {
			//ランニング
			Move(move, worldTransform, player_->GetRunningSpeed());
			playerMotionNo_ = kPlayerMotionRun;
		}
		else {
			playerMotionNo_ = kPlayerMotionWait;
		}

		// 角度補間
		worldTransform->direction_ = Ease::Easing(Ease::EaseName::Lerp, worldTransform->direction_, targetDirection_, targetAngleT_);
	}

	player_->SetReceiveCommand(true);

	//ブロックを叩いたかどうかの値をリセット
	player_->SetIsKnockBlock(false);

}

void PlayerStateRoot::Move(Vector3& move, WorldTransform* worldTransform, float speed)
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
