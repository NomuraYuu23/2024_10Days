#include "TimeOneNumUI.h"

void TimeOneNumUI::Initialize(uint32_t* dayCount, const std::array<uint32_t, 10>& textureHandles, const std::string& groupName, const std::string& jsonName)
{

	dayCount_ = dayCount;

	textureHandles_ = textureHandles;

	Vector2 position = { 0.0f,0.0f };

	Vector4 color = { 1.0f,1.0f,1.0f,1.0f };

	sprite_.reset(Sprite::Create(textureHandles[0], position, color));

	position_ = position;

	rotate_ = 0.0f;

	size_ = sprite_->GetSize();

	jsonName_ = jsonName;

	groupName_ = groupName;

	RegisteringGlobalVariables();

	ApplyGlobalVariables();

}

void TimeOneNumUI::Update()
{

#ifdef _DEMO
	ApplyGlobalVariables();
#endif // _DEMO

	uint32_t one = *dayCount_ % 10;

	sprite_->SetTextureHandle(textureHandles_[one]);

}
