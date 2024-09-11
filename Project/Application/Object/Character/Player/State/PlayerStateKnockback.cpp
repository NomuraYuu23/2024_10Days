#include "PlayerStateKnockback.h"
#include "../Player.h"
#include "../../../../../Engine/Math/Ease.h"
#include "../../../../../Engine/Physics/Gravity.h"

void PlayerStateKnockback::Initialize()
{

	playerMotionNo_ = 0;

	playerStateNo_ = kPlayerStateKnockback;

	// 初めの速さ
	initSpeed_ = 1.0f;

	// 終了フレーム
	endFrame_ = 10;

	// 現在のフレーム
	countFrame_ = 0;

	// 上昇速度
	climbSpeed_ = 0.8f;

	// 初速度設定

	if (player_->GetVelocity().y == 0.0f) {
		// 移動ベクトルをプレイヤーの角度だけ回転する
		Vector3 baseVelocity = player_->GetKnockbackDirection() * initSpeed_;
		velocity_.x = -baseVelocity.x;
		velocity_.z = -baseVelocity.z;

		velocity_.y = climbSpeed_;

		// 加速度設定
		acceleration_ = { baseVelocity.x / static_cast<float>(endFrame_), 0.0f, baseVelocity.z / static_cast<float>(endFrame_) };
	}
	else {
		velocity_ = { 0.0f,0.0f,0.0f };
		acceleration_ = { 0.0f,0.0f,0.0f };
	}

}

void PlayerStateKnockback::Update()
{

	WorldTransform* worldTransform = player_->GetWorldTransformAdress();

	// 減速する
	velocity_ += acceleration_;

	// 移動
	worldTransform->transform_.translate = Vector3::Add(worldTransform->transform_.translate, velocity_);

	// 時間経過確認
	countFrame_++;

	// 終了確認
	if (endFrame_ == countFrame_) {
		// 終了処理
		playerStateNo_ = kPlayerStateRoot;
	}

	// コマンドは受け付けない
	player_->SetReceiveCommand(false);

}
