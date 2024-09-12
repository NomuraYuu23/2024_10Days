#include "TitleSystem.h"
#include "../../../Engine/base/TextureManager.h"
#include "../../../Engine/Input/Input.h"
#include "../../../Engine/Math/Ease.h"
#include "../../../Engine/Math/DeltaTime.h"
#include "../../Camera/GameCamera.h"
#include "../../UI/UISystem.h"

void TitleSystem::Initialize(DirectXCommon* dxCommon, GameCamera* gameCamera, UISystem* UISystem)
{

	// 実行中か
	isRun_ = true;

	// 終了システム中か
	endSystem_ = false;

	// 終了するまでの時間
	endSystemTime_ = 1.0f;

	// 終了するまでの時間 計測用
	currentEndSystemTime_ = 0.0f;

	LogoInitialize(dxCommon);

	GameCameraInitialize(gameCamera);

	UISystemInitialize(UISystem);

}

void TitleSystem::Update()
{

	if (!isRun_) {
		return;
	}

#ifdef _DEMO

	// デバッグ
	if (Input::GetInstance()->TriggerJoystick(JoystickButton::kJoystickButtonSTART)) {
		endSystem_ = true;
	}

#endif // _DEMO

	EndSystem();

	LogoUpdate();

	GameCameraUpdate();

	UISystemUpdate();

}

void TitleSystem::Draw()
{

	if (!isRun_) {
		return;
	}

	titleLogoSprite_->Draw();

}

void TitleSystem::EndSystem()
{

	if (!endSystem_) {
		return;
	}

	currentEndSystemTime_ += kDeltaTime_;

	if (endSystemTime_ <= currentEndSystemTime_) {
		currentEndSystemTime_ = endSystemTime_;
		isRun_ = false;
	}

}

void TitleSystem::LogoInitialize(DirectXCommon* dxCommon)
{

	// 
	Vector2 positon = { 640.0f, 200.0f };
	Vector4 color = { 1.0f,1.0f,1.0f,1.0f };

	// テクスチャハンドル
	titleLogoTextureHandle_ = TextureManager::Load("Resources/Sprite/title.png", dxCommon);

	// スプライト
	titleLogoSprite_.reset(Sprite::Create(titleLogoTextureHandle_, positon, color));

}

void TitleSystem::LogoUpdate()
{

	if (endSystem_) {
		Vector4 color = { 1.0f,1.0f,1.0f,1.0f };

		float t = currentEndSystemTime_ / endSystemTime_;
		color.w = Ease::Easing(Ease::EaseName::Lerp, 1.0f, 0.0f, t);

		titleLogoSprite_->SetColor(color);

	}

}

void TitleSystem::GameCameraInitialize(GameCamera* gameCamera)
{

	gameCamera_ = gameCamera;

}

void TitleSystem::GameCameraUpdate()
{

	if (endSystem_) {
		float t = currentEndSystemTime_ / endSystemTime_;
		gameCamera_->SetTitleCameraOffsetSide(Ease::Easing(Ease::EaseName::EaseInCubic, 0.0f, 1.0f, t));
	}

}

void TitleSystem::UISystemInitialize(UISystem* UISystem)
{

	UISystem_ = UISystem;

}

void TitleSystem::UISystemUpdate()
{

	if (endSystem_) {
		float t = currentEndSystemTime_ / endSystemTime_;
		UISystem_->SetTitlePosAddX(Ease::Easing(Ease::EaseName::EaseInCubic, -640.0f, 0.0f, t));
	}

}
