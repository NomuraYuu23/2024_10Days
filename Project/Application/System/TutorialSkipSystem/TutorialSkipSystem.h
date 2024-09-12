#pragma once
#include <cstdint>

class BlockManager;

class EnemyManager;

class TutorialSkipSystem
{

public:

	void Initialize(BlockManager* blockManager, EnemyManager* enemyManager);

	void Update();

	/// <summary>
	/// 強制終了
	/// </summary>
	void ForcedTermination();

	bool GetCountDownStart() { return countDownStart_; }

private:

	// ブロックマネージャー
	BlockManager* blockManager_;

	// エネミーマネージャー
	EnemyManager* enemyManager_;

	uint32_t blockNumCheck_ = 27; // チェック

	// 終了フラグ
	bool isEnd_;

	// カウントダウン発生フラグ
	bool countDownStart_;

};

