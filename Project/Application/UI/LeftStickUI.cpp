#include "LeftStickUI.h"
#include "../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../Engine/Input/Input.h"

void LeftStickUI::Initialize(uint32_t textureHandle, const std::string& groupName, const std::string& jsonName)
{

	// 移動範囲
	movingRange_ = 5.0f;

	// 入力があるときの色
	inputColor_ = { 1.0f, 1.0f, 1.0f};

	// 入力がないときの色
	notInputColor_ = { 0.75f, 0.75f, 0.75f };

	// 透明度
	alpha_ = 0.75f;

	// 色
	color_ = { notInputColor_.x, notInputColor_.y, notInputColor_.z, alpha_ };

	UI::Initialize(textureHandle, groupName, jsonName);

}

void LeftStickUI::Update()
{

	UI::Update();

	Input* input = Input::GetInstance();
	Vector2 dir = input->GetLeftAnalogstick();
	dir = Vector2::Normalize(dir);
	dir *= movingRange_;

	sprite_->SetPosition(position_ + dir);

	if (dir.x == 0.0f && dir.y == 0.0f) {
		color_ = { notInputColor_.x, notInputColor_.y, notInputColor_.z, alpha_ };
	}
	else {
		color_ = { inputColor_.x, inputColor_.y, inputColor_.z, alpha_ };
	}

	sprite_->SetColor(color_);

}

void LeftStickUI::RegisteringGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	globalVariables->AddItem(jsonName_, groupName_ + "position", position_);
	globalVariables->AddItem(jsonName_, groupName_ + "rotate", rotate_);
	globalVariables->AddItem(jsonName_, groupName_ + "size", size_);
	globalVariables->AddItem(jsonName_, groupName_ + "movingRange", movingRange_);
	globalVariables->AddItem(jsonName_, groupName_ + "inputColor", inputColor_);
	globalVariables->AddItem(jsonName_, groupName_ + "notInputColor", notInputColor_);
	globalVariables->AddItem(jsonName_, groupName_ + "alpha", alpha_);

}

void LeftStickUI::ApplyGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	position_ = globalVariables->GetVector2Value(jsonName_, groupName_ + "position");
	rotate_ = globalVariables->GetFloatValue(jsonName_, groupName_ + "rotate");
	size_ = globalVariables->GetVector2Value(jsonName_, groupName_ + "size");

	movingRange_ = globalVariables->GetFloatValue(jsonName_, groupName_ + "movingRange");
	inputColor_ = globalVariables->GetVector3Value(jsonName_, groupName_ + "inputColor");
	notInputColor_ = globalVariables->GetVector3Value(jsonName_, groupName_ + "notInputColor");
	alpha_ = globalVariables->GetFloatValue(jsonName_, groupName_ + "alpha");

	sprite_->SetPosition(position_);
	sprite_->SetRotate(rotate_);
	sprite_->SetSize(size_);

}
