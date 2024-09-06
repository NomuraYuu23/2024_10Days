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

	if (input_->TriggerJoystick(JoystickButton::kJoystickButtonA)) {
		resultState = PlayerState::kPlayerStateJump;
	}

	if (player_->GetVelocity().y <= Gravity::Execute().y * 2.0f) {
		resultState = PlayerState::kPlayerStateFloating;
	}

	return resultState;

}
