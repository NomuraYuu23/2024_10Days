#include "TutorialSystem.h"

#include "../../../Engine/Object/BaseObjectManager.h"
#include "../../Object/Character/Player/Player.h"
#include "../../Object/Obstacle/Block/BlockManager.h"
#include "../../../Engine/Math/DeltaTime.h"

void TutorialSystem::Initialize(BaseObjectManager* objectManager, Player* player, BlockManager* blockManager)
{

	objectManager_ = objectManager;

	player_ = player;

	blockManager_ = blockManager;

	tutorialFlowNumber_ = kTutorialFlowStartCheck;

	tutorialFlowUpdates_[kTutorialFlowStartCheck] = std::bind(&TutorialSystem::StartCheck, this);
	tutorialFlowUpdates_[kTutorialFlowJumpCheck] = std::bind(&TutorialSystem::JumpCheck, this);
	tutorialFlowUpdates_[kTutorialFlowUpperRowOccurrence] = std::bind(&TutorialSystem::UpperRowOccurrence, this);
	tutorialFlowUpdates_[kTutorialFlowKnockFromBelowCheck] = std::bind(&TutorialSystem::KnockFromBelowCheck, this);
	tutorialFlowUpdates_[kTutorialFlowLowerRowOccurrence] = std::bind(&TutorialSystem::LowerRowOccurrence, this);
	tutorialFlowUpdates_[kTutorialFlowFallingAttackCheck] = std::bind(&TutorialSystem::FallingAttackCheck, this);
	tutorialFlowUpdates_[kTutorialFlowEndSystem] = std::bind(&TutorialSystem::EndSystem, this);

	startCheckStruct_.center_ = blockManager_->GetBlocks()->at(5)->GetWorldTransformAdress()->GetWorldPosition();
	startCheckStruct_.radius_ = 4.0f;

	jumpCheckStruct_.isJumpClear_ = false;
	jumpCheckStruct_.isSmallJumpClear_ = false;

}

void TutorialSystem::Update()
{

	// 現在の更新処理
	tutorialFlowUpdates_[tutorialFlowNumber_]();

}

void TutorialSystem::Draw()
{
}

void TutorialSystem::FallingAttackCheck()
{
}

void TutorialSystem::UpperRowOccurrence()
{
}

void TutorialSystem::LowerRowOccurrence()
{
}

void TutorialSystem::StartCheck()
{

	// 範囲内に入った

	float length = Vector3::Length(player_->GetWorldTransformAdress()->GetWorldPosition() - startCheckStruct_.center_);
	if (length <= startCheckStruct_.radius_) {
		tutorialFlowNumber_ = kTutorialFlowJumpCheck;
	}

}

void TutorialSystem::EndSystem()
{
}

void TutorialSystem::JumpCheck()
{

	if (player_->GetCurrentStateNo() == kPlayerStateJump) {

		double animTimer = player_->GetAnimationAdress()->GetAnimationDatas()->at(kPlayerMotionJump).timer;
		if (animTimer == static_cast<double>(kDeltaTime_) * player_->GetJumpCheckpointFrame()) {
			float playerVelocityY = player_->GetVelocity().y;

			if (playerVelocityY <= player_->GetSmallJumpInitialSpeed()) {
				jumpCheckStruct_.isSmallJumpClear_ = true;
			}
			else{
				jumpCheckStruct_.isJumpClear_ = true;
			}

		}

		if (jumpCheckStruct_.isSmallJumpClear_ && jumpCheckStruct_.isJumpClear_) {
			tutorialFlowNumber_ = kTutorialFlowUpperRowOccurrence;
		}

	}

}

void TutorialSystem::KnockFromBelowCheck()
{
}
