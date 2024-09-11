#pragma once

#include <memory>
#include "../../../Engine/2D/Sprite.h"
#include "../../../Engine/base/DirectXCommon.h"

class TitleSystem
{

public:

	void Initialize(DirectXCommon* dxCommon);

	void Update();

	void Draw();

private:

	void LogoInitialize(DirectXCommon* dxCommon);

	void LogoUpdate();

private:

	// 実行中か
	bool isRun_;

	// 終了システム中か
	bool endSystem_;

	// 終了するまでの時間
	float endSystemTime_;

	// 終了するまでの時間 計測用
	float currentEndSystemTime_;

	// ロゴ
	std::unique_ptr<Sprite> titleLogoSprite_;
	uint32_t titleLogoTextureHandle_;



};

