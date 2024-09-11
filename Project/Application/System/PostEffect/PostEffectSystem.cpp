#include "PostEffectSystem.h"
#include "../../../Engine/Math/Ease.h"
#include "../../../Engine/Math/DeltaTime.h"
#include "../../../Engine/base/WindowSprite.h"

void PostEffectSystem::Initialize(Player* player)
{

	postEffect_ = PostEffect::GetInstance();

	// プレイヤー
	player_ = player;

	// ダメージエフェクト
	damage_.Initialize(this);

}

void PostEffectSystem::Update()
{

	uint32_t executionFlag = 0;

	damage_.Update();
	if (damage_.isRun_) {
		executionFlag += damage_.num_;
	}

	postEffect_->SetExecutionFlag(executionFlag);

}

void PostEffectSystem::Draw(DirectXCommon* dxCommon, RenderTargetTexture* renderTargetTexture)
{

	// ポストエフェクト

	PostEffect::GetInstance()->Execution(
		dxCommon->GetCommadList(),
		renderTargetTexture,
		PostEffect::kCommandIndexDOROKERA
	);

	WindowSprite::GetInstance()->DrawSRV(PostEffect::GetInstance()->GetEditTextures(0));

}

void PostEffectSystem::Damage::Initialize(PostEffectSystem* parent)
{

	time_ = 0.0f;
	
	endTime_ = 0.5f;
	
	maxChange_ = 1.0f;
	
	minChange_ = 0.0f;

	isRun_ = false;

	num_ = 1;

	parent_ = parent;

}

void PostEffectSystem::Damage::Update()
{

	isRun_ = (parent_->player_->GetReceiveDamage() || time_ != 0.0f);

	if (isRun_) {
		time_ += kDeltaTime_;
		if (time_ >= endTime_) {
			time_ = 0.0f;
			isRun_ = false;
			parent_->postEffect_->SetVignetteChange(0.0f);
		}
		else {

			float t = time_ / endTime_;

			if (time_ <= endTime_ / 2.0f) {
				t *= 2.0f;
			}
			else {
				t = (1.0f - t) * 2.0f;
			}

			parent_->postEffect_->SetVignetteChange(Ease::Easing(Ease::EaseName::Lerp, minChange_, maxChange_, t));

		}
	}

}
