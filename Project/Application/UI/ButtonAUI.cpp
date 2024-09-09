#include "ButtonAUI.h"
#include "../../Engine/Input/Input.h"
#include "../../Engine/GlobalVariables/GlobalVariables.h"

void ButtonAUI::Initialize(uint32_t textureHandle, const std::string& groupName, const std::string& jsonName)
{

	// 入力があるときの色
	inputColor_ = { 1.0f, 1.0f, 1.0f };

	// 入力がないときの色
	notInputColor_ = { 0.75f, 0.75f, 0.75f };

	// 透明度
	alpha_ = 1.0f;

	// 色
	color_ = { notInputColor_.x, notInputColor_.y, notInputColor_.z, alpha_ };

	UI::Initialize(textureHandle, groupName, jsonName);

}

void ButtonAUI::Update()
{

	UI::Update();

	Input* input = Input::GetInstance();

	if (!input->PushJoystick(JoystickButton::kJoystickButtonA)) {
		color_ = { notInputColor_.x, notInputColor_.y, notInputColor_.z, alpha_ };
	}
	else {
		color_ = { inputColor_.x, inputColor_.y, inputColor_.z, alpha_ };
	}

	sprite_->SetColor(color_);

}

void ButtonAUI::RegisteringGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	globalVariables->AddItem(jsonName_, groupName_ + "position", position_);
	globalVariables->AddItem(jsonName_, groupName_ + "rotate", rotate_);
	globalVariables->AddItem(jsonName_, groupName_ + "size", size_);

	globalVariables->AddItem(jsonName_, groupName_ + "inputColor", inputColor_);
	globalVariables->AddItem(jsonName_, groupName_ + "notInputColor", notInputColor_);
	globalVariables->AddItem(jsonName_, groupName_ + "alpha", alpha_);

}

void ButtonAUI::ApplyGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	position_ = globalVariables->GetVector2Value(jsonName_, groupName_ + "position");
	rotate_ = globalVariables->GetFloatValue(jsonName_, groupName_ + "rotate");
	size_ = globalVariables->GetVector2Value(jsonName_, groupName_ + "size");

	inputColor_ = globalVariables->GetVector3Value(jsonName_, groupName_ + "inputColor");
	notInputColor_ = globalVariables->GetVector3Value(jsonName_, groupName_ + "notInputColor");
	alpha_ = globalVariables->GetFloatValue(jsonName_, groupName_ + "alpha");

	sprite_->SetPosition(position_);
	sprite_->SetRotate(rotate_);
	sprite_->SetSize(size_);

}
