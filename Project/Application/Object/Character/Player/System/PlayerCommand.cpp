#include "PlayerCommand.h"
#include "../State/IPlayerState.h"

Input* PlayerCommand::input_ = Input::GetInstance();

PlayerCommand* PlayerCommand::GetInstance()
{
	static PlayerCommand instance;
	return &instance;
}

void PlayerCommand::Initialize()
{
}

uint32_t PlayerCommand::Command()
{

	uint32_t resultState = PlayerState::kPlayerStateRoot;

	if (input_->TriggerJoystick(JoystickButton::kJoystickButtonA)) {
		resultState = PlayerState::kPlayerStateJump;
	}

	return resultState;

}
