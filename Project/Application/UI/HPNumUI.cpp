#include "HPNumUI.h"
#include "../Object/Character/Player/Player.h"

void HPNumUI::Initialize(Player* player, const std::array<uint32_t, 4>& textureHandles, const std::string& groupName, const std::string& jsonName)
{

	player_ = player;

	textureHandles_ = textureHandles;
	
	Vector2 position = { 0.0f,0.0f };

	Vector4 color = { 1.0f,1.0f,1.0f,1.0f };

	sprite_.reset(Sprite::Create(textureHandles[3], position, color));

	position_ = position;

	rotate_ = 0.0f;

	size_ = sprite_->GetSize();

	jsonName_ = jsonName;

	groupName_ = groupName;

	RegisteringGlobalVariables();

	ApplyGlobalVariables();

}

void HPNumUI::Update()
{

#ifdef _DEMO
	ApplyGlobalVariables();
#endif // _DEMO

	sprite_->SetTextureHandle(textureHandles_[player_->GetHp()]);

}
