#include "HPUI.h"
#include "../../Engine/GlobalVariables/GlobalVariables.h"
#include "../Object/Character/Player/Player.h"

void HPUI::Initialize(Player* player, uint32_t displayValue, uint32_t textureHandle, const std::string& groupName, const std::string& jsonName)
{

	player_ = player;

	displayValue_ = displayValue;

	isDead_ = false;

	UI::Initialize(textureHandle, groupName, jsonName);

}

void HPUI::Update()
{

	UI::Update();

	if(sprite_->GetIsInvisible()){
		return;
	}

	if (displayValue_ > player_->GetHp() && !isDead_) {
		isDead_ = true;
		acceleration_ = { 0.0f, 1.0f };
		velocity_ = { 2.0f, -10.0f };
	}

	if (isDead_) {

		rotate_ += 0.4f;
		sprite_->SetRotate(rotate_);

		velocity_ += acceleration_;
		position_ += velocity_;
		sprite_->SetPosition(position_);
		if (position_.y > 800.0f) {
			sprite_->SetIsInvisible(true);
		}

	}

}

void HPUI::RegisteringGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	globalVariables->AddItem(jsonName_, groupName_ + std::to_string(displayValue_) + "position", position_);
	globalVariables->AddItem(jsonName_, groupName_ + std::to_string(displayValue_) + "size", size_);

}

void HPUI::ApplyGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	if (!isDead_) {
		position_ = globalVariables->GetVector2Value(jsonName_, groupName_ + std::to_string(displayValue_) + "position");
		size_ = globalVariables->GetVector2Value(jsonName_, groupName_ + std::to_string(displayValue_) + "size");
		sprite_->SetPosition(position_);
		sprite_->SetSize(size_);
	}

}
