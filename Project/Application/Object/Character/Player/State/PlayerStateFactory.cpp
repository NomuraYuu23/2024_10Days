#include "PlayerStateFactory.h"

#include "PlayerStateRoot.h"

PlayerStateFactory* PlayerStateFactory::GetInstance()
{
	static PlayerStateFactory instance;
	return &instance;
}

IPlayerState* PlayerStateFactory::CreatePlayerState(uint32_t playerStateName)
{
	IPlayerState* newPlayerState = nullptr;

	switch (playerStateName)
	{
	case kPlayerStateRoot: // 通常
		newPlayerState = new PlayerStateRoot();
		break;
	case kPlayerStateOfCount: // 使用不可
	default:
		assert(0);
		break;
	}

	return newPlayerState;
}
