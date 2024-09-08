#include "BackGround.h"

void BackGround::Initialize(uint32_t textureHandle)
{

	sprite_ = Sprite::Create(textureHandle, Vector2{ 640.0f, 360.0f }, Vector4{ 1.0f,1.0f,1.0f,1.0f });
	sprite_->SetSize(Vector2{ 1280.0f, 720.0f });

}

void BackGround::Update(const Vector4& color)
{

	sprite_->SetColor(color);
	sprite_->Update();

}

void BackGround::Draw()
{

	sprite_->Draw();

}
