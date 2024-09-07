#include "PlayerStateHeadDrop.h"
#include "../Player.h"
#include "../../../../../Engine/Math/Ease.h"
#include "../../../../../Engine/Math/DeltaTime.h"

void PlayerStateHeadDrop::Initialize()
{

	playerMotionNo_ = kPlayerMotionWait;

	playerStateNo_ = kPlayerStateHeadDrop;

	// 開始地点 (現在のプレイヤーの位置)
	startPos_ = player_->GetWorldTransformAdress()->GetWorldPosition();

	// 終了地点 (開始地点のy座標を下まで)
	endPos_ = startPos_;
	endPos_.y = 0.0f;

	// 落下終了までの時間
	endTime_ = 0.5f;

	// 経過時間
	time_ = 0.0f;

}

void PlayerStateHeadDrop::Update()
{

	// 時間を進める
	time_ += kDeltaTime_;

	// 補間係数決定
	float t = time_ / endTime_;
	t = fminf(t, 1.0f);

	// 位置変更
	WorldTransform* worldTransform = player_->GetWorldTransformAdress();
	worldTransform->usedDirection_ = true;

	worldTransform->transform_.translate = Ease::Easing(Ease::EaseName::EaseInOutCirc, startPos_, endPos_, t);

	// コマンドは受け付けない
	player_->SetReceiveCommand(false);

	// 落下終了したらRootへ
	if (t == 1.0f) {
		playerStateNo_ = kPlayerStateRoot;
	}

}
