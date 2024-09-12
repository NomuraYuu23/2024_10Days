#include "TutorialSystem.h"

#include "../../Object/Manager/GameSceneObjectManager.h"
#include "../../Object/Character/Player/Player.h"
#include "../../../Engine/Math/DeltaTime.h"

void TutorialSystem::Initialize(GameSceneObjectManager* gameSceneObjectManager, Player* player)
{

	gameSceneObjectManager_ = gameSceneObjectManager;

	player_ = player;

	tutorialFlowNumber_ = kTutorialFlowStartCheck;

	tutorialFlowUpdates_[kTutorialFlowStartCheck] = std::bind(&TutorialSystem::StartCheck, this);
	tutorialFlowUpdates_[kTutorialFlowJumpCheck] = std::bind(&TutorialSystem::JumpCheck, this);
	tutorialFlowUpdates_[kTutorialFlowUpperRowOccurrence] = std::bind(&TutorialSystem::UpperRowOccurrence, this);
	tutorialFlowUpdates_[kTutorialFlowKnockFromBelowCheck] = std::bind(&TutorialSystem::KnockFromBelowCheck, this);
	tutorialFlowUpdates_[kTutorialFlowLowerRowOccurrence] = std::bind(&TutorialSystem::LowerRowOccurrence, this);
	tutorialFlowUpdates_[kTutorialFlowFallingAttackCheck] = std::bind(&TutorialSystem::FallingAttackCheck, this);
	tutorialFlowUpdates_[kTutorialFlowEndSystem] = std::bind(&TutorialSystem::EndSystem, this);

	startCheckStruct_.center_;
	startCheckStruct_.radius_;

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
	if (Vector3::Length(player_->GetWorldTransformAdress()->GetWorldPosition() - startCheckStruct_.center_) <= startCheckStruct_.radius_) {
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

			if (playerVelocityY == player_->GetSmallJumpInitialSpeed()) {
				jumpCheckStruct_.isSmallJumpClear_ = true;
			}
			else if(playerVelocityY == player_->GetJumpInitialSpeed()){
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
