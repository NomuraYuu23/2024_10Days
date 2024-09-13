#include "GameClear.h"

#include "../../../Engine/Input/Input.h"
#include "../../../Engine/base/TextureManager.h"
#include "../../../Engine/Math/Ease.h"
#include "../../../Engine/Math/DeltaTime.h"

void GameClear::Initialize()
{

	// 実行
	isRun_ = false;

	// 終了
	isEnd_ = false;

	Vector4 color = { 1.0f,1.0f, 1.0f ,1.0f };

	Vector2 size = { 1152.0f,400.0f };

	Vector2 pos = { 640.0f, 300.0f };

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	gameclearSprite_.reset(Sprite::Create(TextureManager::Load("Resources/Sprite/Clear.png", dxCommon), pos, color));
	gameclearSprite_->SetSize(size);

	size *= 0.25f;
	pos.y += 300.0f;
	pos.x += 72.0f;
	goTitleSprite_.reset(Sprite::Create(TextureManager::Load("Resources/Sprite/Back_Title.png", dxCommon), pos, color));
	goTitleSprite_->SetSize(size);

	size = { 100.0f,100.0f };
	pos.x -= 216.0f;
	buttonSprite_.reset(Sprite::Create(TextureManager::Load("Resources/UI/buttonA.png", dxCommon), pos, color));
	buttonSprite_->SetSize(size);

	size = { 1280.0f,720.0f };
	pos = { 640.0f,360.0f };
	color = { 0.0f,0.0f,0.0f,0.9f };
	blockSprite_.reset(Sprite::Create(TextureManager::Load("Resources/default/white2x2.png", dxCommon), pos, color));
	blockSprite_->SetSize(size);

}

void GameClear::Update(bool isGameClear)
{

	if (!isGameClear) {
		return;
	}

	graceTimer_ += kDeltaTime_;
	if (graceTimer_ >= graceTime_) {
		graceTimer_ = graceTime_;
	}
	else {
		Vector4 color = { 0.0f,0.0f,0.0f,0.0f };
		color.w = Ease::Easing(Ease::EaseName::Lerp, 0.0f, 0.9f, graceTimer_ / graceTime_);
		blockSprite_->SetColor(color);

		color = { 1.0f, 1.0f, 1.0f, color.w };
		gameclearSprite_->SetColor(color);
		goTitleSprite_->SetColor(color);
		buttonSprite_->SetColor(color);

		return;
	}

	// ゲームクリア
	isRun_ = true;

	Input* input = Input::GetInstance();

	if (input->GetJoystickConnected()) {
		if (input->TriggerJoystick(JoystickButton::kJoystickButtonA)) {
			isEnd_ = true;
		}
	}
	else {
		if (input->TriggerKey(DIK_SPACE)) {
			isEnd_ = true;
		}
	}

}

void GameClear::Draw()
{

	if (graceTimer_ == 0.0f) {
		return;
	}

	blockSprite_->Draw();

	gameclearSprite_->Draw();

	goTitleSprite_->Draw();

	buttonSprite_->Draw();

}
