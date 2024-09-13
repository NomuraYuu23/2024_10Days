#include "GameOver.h"

#include "../../../Engine/Input/Input.h"
#include "../../../Engine/base/TextureManager.h"

void GameOver::Initialize()
{

	// 実行
	isRun_ = false;

	// 終了
	isEnd_ = false;

	Vector4 color = {1.0f,1.0f, 1.0f ,1.0f};

	Vector2 size = { 1152.0f,400.0f };

	Vector2 pos = { 640.0f, 300.0f };

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	gameoverSprite_.reset(Sprite::Create(TextureManager::Load("Resources/Sprite/GameOver.png", dxCommon), pos, color));
	gameoverSprite_->SetSize(size);

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

void GameOver::Update(bool isGameOver)
{

	if (!isGameOver) {
		return;
	}

	// ゲームオーバー
	isRun_ = true;

	if (Input::GetInstance()->TriggerJoystick(JoystickButton::kJoystickButtonA)) {
		isEnd_ = true;
	}

}

void GameOver::Draw()
{

	blockSprite_->Draw();

	gameoverSprite_->Draw();

	goTitleSprite_->Draw();

	buttonSprite_->Draw();

}
