#include "TutorialSystem.h"

#include "../../Object/Character/Player/Player.h"

void TutorialSystem::StartCheck()
{

	if (Vector3::Length(player_->GetWorldTransformAdress()->GetWorldPosition() - startCheckStruct_.center_) <= startCheckStruct_.radius_) {
		startCheckStruct_.isClear_ = true;
	}

}
