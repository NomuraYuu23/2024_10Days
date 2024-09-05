#include "PlayerCommand.h"
#include "../State/IPlayerState.h"

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

	return resultState;

}
