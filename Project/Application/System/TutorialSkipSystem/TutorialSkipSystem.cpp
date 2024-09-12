#include "TutorialSkipSystem.h"
#include "../../Object/Character/Enemy/EnemyManager.h"
#include "../../Object/Obstacle/Block/BlockManager.h"

void TutorialSkipSystem::Initialize(BlockManager* blockManager, EnemyManager* enemyManager)
{

	// 変数設定
	blockManager_ = blockManager;
	enemyManager_ = enemyManager;

	// 卵生成
	enemyManager_->AddEgg(blockManager_->GetBlocks()->at(blockNumCheck_)->GetWorldTransformAdress()->GetWorldPosition());

}

void TutorialSkipSystem::Update()
{
	
	if (isEnd_) {
		return;
	}

	// ブロックたたかれた
	if (blockManager_->GetBlocks()->at(blockNumCheck_)->GetIsAttack()) {
		// カウントダウン起動
		countDownStart_ = true;
	}

}

void TutorialSkipSystem::ForcedTermination()
{

	isEnd_ = true;

	//卵削除したい

}
