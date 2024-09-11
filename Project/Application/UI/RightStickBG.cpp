#include "RightStickBG.h"
#include "RightStickUI.h"

void RightStickBG::Initialize(uint32_t textureHandle, const std::string& groupName, const std::string& jsonName)
{

	Vector2 position = { 0.0f,0.0f };

	Vector4 color = { 0.5f,0.5f,0.5f,1.0f };

	sprite_.reset(Sprite::Create(textureHandle, position, color));

	position_ = position;

	rotate_ = 0.0f;

	size_ = sprite_->GetSize();

	jsonName_ = jsonName;

	groupName_ = groupName;

}

void RightStickBG::Update()
{

	position_ = parent_->GetPosition();
	size_ = parent_->GetSize();

	sprite_->SetPosition(position_);
	sprite_->SetSize(size_);

	Vector4 color = parent_->GetColor();

	sprite_->SetColor(color);

}

void RightStickBG::SetParent(RightStickUI* parent)
{

	parent_ = parent;

}
