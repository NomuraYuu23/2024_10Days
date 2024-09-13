#include "PlayerCommand.h"
#include "../State/IPlayerState.h"
#include "../Player.h"
#include "../../../../../Engine/Physics/Gravity.h"

Input* PlayerCommand::input_ = Input::GetInstance();

Player* PlayerCommand::player_ = nullptr;

PlayerCommand* PlayerCommand::GetInstance()
{
	static PlayerCommand instance;
	return &instance;
}

void PlayerCommand::Initialize(Player* player)
{

	player_ = player;

}

uint32_t PlayerCommand::Command()
{

	uint32_t resultState = PlayerState::kPlayerStateRoot;

	if (input_->GetJoystickConnected()) {
		if (input_->TriggerJoystick(JoystickButton::kJoystickButtonA)) {
			resultState = PlayerState::kPlayerStateJump;
		}
	}
	else {
		if (input_->TriggerKey(DIK_SPACE)) {
			resultState = PlayerState::kPlayerStateJump;
		}
	}

	return resultState;

}
