#include "PlayerStateFloating.h"
#include "../Player.h"
#include "../../../../../Engine/Math/Ease.h"

void PlayerStateFloating::Initialize()
{

	targetDirection_ = { 0.0f, 0.0f, 1.0f };

	targetAngleT_ = 0.1f;

	playerMotionNo_ = kPlayerMotionWait;

	playerStateNo_ = kPlayerStateFloating;

}

void PlayerStateFloating::Update()
{

	WorldTransform* worldTransform = player_->GetWorldTransformAdress();

	worldTransform->usedDirection_ = true;
	targetDirection_ = worldTransform->direction_;

	Vector3 fallPosition = player_->GetFallingPosition();

	if (fallPosition.x != 1000.0f &&
		fallPosition.y != 1000.0f &&
		fallPosition.z != 1000.0f) {

		float tmpY = worldTransform->transform_.translate.y;

		worldTransform->transform_.translate = Ease::Easing(Ease::EaseName::Lerp, worldTransform->GetWorldPosition(), player_->GetFallingPosition(), 0.02f);

		worldTransform->transform_.translate.y = tmpY;

	}

	player_->SetReceiveCommand(false);

	if (player_->GetVelocity().y >= 0.0f) {
		playerStateNo_ = kPlayerStateRoot;
		player_->SetFallingPosition(Vector3{1000.0f, 1000.0f , 1000.0f });
	}


}

void PlayerStateFloating::Move(Vector3& move, WorldTransform* worldTransform, float speed)
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
