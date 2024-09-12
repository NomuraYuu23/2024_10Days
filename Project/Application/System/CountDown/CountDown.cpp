#include "CountDown.h"
#include "../../../Engine/base/DirectXCommon.h"
#include "../../../Engine/base/TextureManager.h"
#include "../../../Engine/Math/DeltaTime.h"

void CountDown::Initialize()
{

	countDownFlowNumber_ = kCountDownFlowMain;

	countDownFlowUpdates_[kCountDownFlowMain] = std::bind(&CountDown::Main, this);
	countDownFlowUpdates_[kCountDownFlowGameStart] = std::bind(&CountDown::GameStart, this);
	countDownFlowUpdates_[kCountDownFlowEndSystem] = std::bind(&CountDown::EndSystem, this);

	SpriteInitialize();

	// 実行フラグ
	isRun_ = false;

	// ゲーム開始フラグ
	isGameStart_ = false;

	count_ = kCountMax_;

}

void CountDown::Update()
{

	if (!isRun_) {
		return;
	}

	// 現在の更新処理
	countDownFlowUpdates_[countDownFlowNumber_]();

}

void CountDown::Draw()
{

	if (!isRun_) {
		return;
	}

	if (isGameStart_) {
		gameStartString_->Draw();
	}
	else {
		countNum_->Draw();
	}

}

void CountDown::SpriteInitialize()
{

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Vector4 color = { 1.0f,1.0f,1.0f,1.0f };

	// カウント数字
	countNumPos_ = { 320.0f, 250.0f };
	countNumSize_ = { 64.0f, 64.0f };
	numTextureHandles_[0] = TextureManager::Load("Resources/Sprite/nums/1.png", dxCommon);
	numTextureHandles_[1] = TextureManager::Load("Resources/Sprite/nums/2.png", dxCommon);
	numTextureHandles_[2] = TextureManager::Load("Resources/Sprite/nums/3.png", dxCommon);
	countNum_.reset(Sprite::Create(numTextureHandles_[2], countNumPos_, color));
	countNum_->SetSize(countNumSize_);

	// ゲームスタート文字
	gameStartStringPos_ = { 320.0f, 250.0f };
	gameStartStringSize_ = { 576.0f, 100.0f};
	uint32_t textureHandle = TextureManager::Load("Resources/Sprite/start_UI.png", dxCommon);
	gameStartString_.reset(Sprite::Create(textureHandle, gameStartStringPos_, color));
	gameStartString_->SetSize(gameStartStringSize_);
	gameStartString_->SetTextureLeftTop(Vector2{ 0.0f, 222.0f });
	gameStartString_->SetTextureSize(Vector2{ 1152.0f, 178.0f });


}

void CountDown::Main()
{

	// タイム
	elapsedTime_ += kDeltaTime_;
	if (elapsedTime_ >= timeMax_) {
		elapsedTime_ = 0.0f;
		count_--;
		if (count_ == 0) {
			countDownFlowNumber_ = kCountDownFlowGameStart;
			isGameStart_ = true;
		}
		else {
			countNum_->SetTextureHandle(numTextureHandles_[count_ - 1]);
		}
	}


}

void CountDown::GameStart()
{

	// タイム
	elapsedTime_ += kDeltaTime_;
	if (elapsedTime_ >= timeMax_) {
		elapsedTime_ = 0.0f;
		countDownFlowNumber_ = kCountDownFlowEndSystem;
	}

}


void CountDown::EndSystem()
{

	isRun_ = false;

}
