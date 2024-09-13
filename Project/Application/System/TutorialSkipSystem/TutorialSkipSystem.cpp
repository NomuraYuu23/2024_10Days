#include "TutorialSkipSystem.h"
#include "../../Object/Character/Enemy/EnemyManager.h"
#include "../../Object/Obstacle/Block/BlockManager.h"
#include "../../../Engine/base/DirectXCommon.h"
#include "../../../Engine/base/TextureManager.h"
#include "../../Object/Character/Enemy/Egg.h"

void TutorialSkipSystem::Initialize(BlockManager* blockManager, EnemyManager* enemyManager)
{

	// 変数設定
	blockManager_ = blockManager;
	enemyManager_ = enemyManager;

	// 卵生成
	enemyManager_->AddEgg(blockManager_->GetBlocks()->at(blockNumCheck_)->GetWorldTransformAdress()->GetWorldPosition());

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// ゲームスタート文字
	Vector4 color = { 1.0f,1.0f,1.0f,1.0f };
	Vector2 pos = { 275.0f, 500.0f };
	Vector2 size = { 576.0f, 200.0f };
	uint32_t textureHandle = TextureManager::Load("Resources/Sprite/start_UI.png", dxCommon);
	sprite_.reset(Sprite::Create(textureHandle, pos, color));
	sprite_->SetSize(size);

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
		isEnd_ = true;
	}

}

void TutorialSkipSystem::Draw()
{

	if (isEnd_) {
		return;
	}

	sprite_->Draw();

}

void TutorialSkipSystem::ForcedTermination()
{

	if (isEnd_) {
		return;
	}

	isEnd_ = true;

	//卵削除したい
	Egg* egg = *(enemyManager_->GetEggs()->begin());
	egg->Delete();
	//enemyManager_->RemoveEgg(egg);

}
