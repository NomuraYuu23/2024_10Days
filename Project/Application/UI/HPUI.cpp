#include "HPUI.h"
#include "../../Engine/GlobalVariables/GlobalVariables.h"
#include "../Object/Character/Player/Player.h"

Vector2 HPUI::basePosition_ = { 0.0f,0.0f };

float HPUI::space_ = 0.0f;

void HPUI::Initialize(Player* player, uint32_t displayValue, uint32_t textureHandle, const std::string& groupName, const std::string& jsonName)
{

	player_ = player;

	displayValue_ = displayValue;

	UI::Initialize(textureHandle, groupName, jsonName);

}

void HPUI::Update()
{

	UI::Update();

	if (displayValue_ > player_->GetHp()) {
		sprite_->SetIsInvisible(true);
	}

}

void HPUI::RegisteringGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	globalVariables->AddItem(jsonName_, groupName_  + "basePosition", basePosition_);
	globalVariables->AddItem(jsonName_, groupName_ + "space", space_);
	globalVariables->AddItem(jsonName_, groupName_ + "size", size_);

}

void HPUI::ApplyGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	basePosition_ = globalVariables->GetVector2Value(jsonName_, groupName_ + "basePosition");
	space_ = globalVariables->GetFloatValue(jsonName_, groupName_ + "space");
	size_ = globalVariables->GetVector2Value(jsonName_, groupName_ + "size");

	Vector2 pos = basePosition_;
	pos.x += space_ * static_cast<float>(displayValue_ - 1);
	sprite_->SetPosition(pos);

	sprite_->SetSize(size_);

}
