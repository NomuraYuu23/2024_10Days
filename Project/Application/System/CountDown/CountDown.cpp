#include "CountDown.h"
#include "../../../Engine/base/DirectXCommon.h"
#include "../../../Engine/base/TextureManager.h"

void CountDown::Initialize()
{

	countDownFlowNumber_ = kCountDownFlowCome;

	countDownFlowUpdates_[kCountDownFlowCome] = std::bind(&CountDown::Come, this);
	countDownFlowUpdates_[kCountDownFlowMain] = std::bind(&CountDown::Main, this);
	countDownFlowUpdates_[kCountDownFlowGameStart] = std::bind(&CountDown::GameStart, this);
	countDownFlowUpdates_[kCountDownFlowGoHome] = std::bind(&CountDown::GoHome, this);
	countDownFlowUpdates_[kCountDownFlowEndSystem] = std::bind(&CountDown::EndSystem, this);

	SpriteInitialize();

}

void CountDown::Update()
{

	if (isEnd_) {
		return;
	}

	// 現在の更新処理
	countDownFlowUpdates_[countDownFlowNumber_]();

}

void CountDown::Draw()
{

	//countNum_->Draw();
	//gameStartString_->Draw();

}

void CountDown::SpriteInitialize()
{

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Vector4 color = { 1.0f,1.0f,1.0f,1.0f };

	// カウント数字
	countNumPos_ = {};
	countNumSize_ = {};
	numTextureHandles_[0] = TextureManager::Load("Resources/Sprite/nums/1.png", dxCommon);
	numTextureHandles_[1] = TextureManager::Load("Resources/Sprite/nums/2.png", dxCommon);
	numTextureHandles_[2] = TextureManager::Load("Resources/Sprite/nums/3.png", dxCommon);
	countNum_.reset(Sprite::Create(numTextureHandles_[2], countNumPos_, color));
	countNum_->SetSize(countNumSize_);

	// ゲームスタート文字
	gameStartStringPos_ = {};
	gameStartStringSize_ = {};
	uint32_t textureHandle = TextureManager::Load("Resources/Sprite/start_UI.png", dxCommon);
	gameStartString_.reset(Sprite::Create(textureHandle, gameStartStringPos_, color));
	gameStartString_->SetSize(gameStartStringSize_);

}

void CountDown::Come()
{
}

void CountDown::Main()
{
}

void CountDown::GameStart()
{
}

void CountDown::GoHome()
{
}

void CountDown::EndSystem()
{
}
