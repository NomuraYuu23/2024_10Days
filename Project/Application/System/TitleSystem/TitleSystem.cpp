#include "TitleSystem.h"
#include "../../../Engine/base/TextureManager.h"

void TitleSystem::Initialize(DirectXCommon* dxCommon)
{

	// 実行中か
	isRun_ = true;

	// 終了システム中か
	endSystem_ = false;

	// 終了するまでの時間
	endSystemTime_ = 3.0f;

	// 終了するまでの時間 計測用
	currentEndSystemTime_ = 0.0f;

	LogoInitialize(dxCommon);

}

void TitleSystem::Update()
{

	LogoUpdate();

}

void TitleSystem::Draw()
{

	if (!isRun_) {
		return;
	}

	titleLogoSprite_->Draw();

}

void TitleSystem::LogoInitialize(DirectXCommon* dxCommon)
{

	// 
	Vector2 positon = {640.0f,200.0f};
	Vector4 color = { 1.0f,1.0f,1.0f,1.0f };

	// テクスチャハンドル
	titleLogoTextureHandle_ = TextureManager::Load("Resources/Sprite/title.png", dxCommon);

	// スプライト
	titleLogoSprite_.reset(Sprite::Create(titleLogoTextureHandle_, positon, color));

}

void TitleSystem::LogoUpdate()
{

	if (endSystem_) {

	}

}
