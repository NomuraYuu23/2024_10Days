#pragma once
#include "../../../Engine/PostEffect/PostEffect.h"
#include "../../Object/Character/Player/Player.h"

class PostEffectSystem
{

private:

	struct Damage
	{

		void Initialize(PostEffectSystem* parent);
		void Update();

		float time_;
		float endTime_;
		float maxChange_;
		float minChange_;

		bool isRun_; // 起動
		PostEffectSystem* parent_;

		uint32_t num_; //番号

	};

public:

	void Initialize(Player* player);

	void Update();
	
	void Draw(DirectXCommon* dxCommon, RenderTargetTexture* renderTargetTexture);

private:

	PostEffect* postEffect_;

	// プレイヤー
	Player* player_;

	// ダメージエフェクト
	Damage damage_;

};

