#include "PadConnect.h"
#include "../../../Engine/base/TextureManager.h"

void PadConnect::Initialize()
{

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	pos_ = {1000.0f,600.0f};

	size_ = {288.0f, 100.0f};

	uint32_t textureHandle = TextureManager::Load("Resources/Sprite/PAD.png", dxCommon);;
	Vector4 color = { 1.0f,1.0f,1.0f,1.0f };
	sprite_.reset(Sprite::Create(textureHandle, pos_, color));
	sprite_->SetSize(size_);

	input_ = Input::GetInstance();

	isRun_ = true;

}

void PadConnect::Update()
{

	if (!isRun_ || input_->GetJoystickConnected()) {
		sprite_->SetIsInvisible(true);
	}
	else {
		sprite_->SetIsInvisible(false);
	}

}

void PadConnect::Draw()
{

	sprite_->Draw();

}
