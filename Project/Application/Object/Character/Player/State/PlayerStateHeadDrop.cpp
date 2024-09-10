#include "PlayerStateHeadDrop.h"
#include "../Player.h"
#include "../../../../../Engine/Math/Ease.h"
#include "../../../../../Engine/Math/DeltaTime.h"

void PlayerStateHeadDrop::Initialize()
{

	playerMotionNo_ = kPlayerMotionHeadDrop;

	playerStateNo_ = kPlayerStateHeadDrop;

	// 開始地点 (現在のプレイヤーの位置)
	startPos_ = player_->GetWorldTransformAdress()->GetWorldPosition();

	// 終了地点 (開始地点のy座標を下まで)
	endPos_ = startPos_;
	endPos_.y = 0.0f;

	// 落下終了までの時間
	fallEndTime_ = 0.5f;

	// 状態終了までの時間
	stateEndTime_ = 0.75f;

	// 経過時間
	time_ = 0.0f;

}

void PlayerStateHeadDrop::Update()
{

	// 時間を進める
	time_ += kDeltaTime_;

	// 補間係数決定
	float t = time_ / fallEndTime_;

	// 位置変更
	WorldTransform* worldTransform = player_->GetWorldTransformAdress();
	worldTransform->usedDirection_ = true;

	float tmpY = startPos_.y;

	startPos_ = Ease::Easing(Ease::EaseName::Lerp, worldTransform->GetWorldPosition(), player_->GetFallingPosition(), 0.2f);

	startPos_.y = tmpY;

	endPos_ = startPos_;
	endPos_.y = 0.0f;

	worldTransform->transform_.translate = Ease::Easing(Ease::EaseName::EaseInCirc, startPos_, endPos_, fminf(t, 1.0f));

	// コマンドは受け付けない
	player_->SetReceiveCommand(false);

	// 終了時間になったらRootへ
	if (time_ >= stateEndTime_) {
		playerStateNo_ = kPlayerStateRoot;
	}

	// アニメーション確認
	Animation* animation = player_->GetAnimationAdress();
	double animEndTime = animation->GetAnimationDatas()->at(kPlayerMotionHeadDrop).animation.endTime_;
	double animTimer = animation->GetAnimationDatas()->at(kPlayerMotionHeadDrop).timer;

	if (animTimer > animEndTime - kDeltaTime_) {
		animTimer = animEndTime - kDeltaTime_;
		animation->AnimationTimerFix(kPlayerMotionHeadDrop, animTimer);
	}

}
