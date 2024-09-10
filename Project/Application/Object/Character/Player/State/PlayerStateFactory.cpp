#include "PlayerStateFactory.h"

#include "PlayerStateRoot.h"
#include "PlayerStateJump.h"
#include "PlayerStateFloating.h"
#include "PlayerStateHeadDrop.h"
#include "PlayerStateKnockback.h"

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
	case kPlayerStateJump: // ジャンプ
		newPlayerState = new PlayerStateJump();
		break;
	case kPlayerStateFloating: // 浮いている状態
		newPlayerState = new PlayerStateFloating();
		break;
	case kPlayerStateHeadDrop: // ヘッドドロップ状態
		newPlayerState = new PlayerStateHeadDrop();
		break;
		break;
	case kPlayerStateKnockback: // ヘッドドロップ状態
		newPlayerState = new PlayerStateKnockback();
		break;
	case kPlayerStateOfCount: // 使用不可
	default:
		assert(0);
		break;
	}

	return newPlayerState;
}
