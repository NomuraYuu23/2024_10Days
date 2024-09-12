#pragma once
#include <cstdint>
#include "../../../Engine/2D/Sprite.h"

class BlockManager;

class EnemyManager;

class TutorialSkipSystem
{

public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="blockManager"></param>
	/// <param name="enemyManager"></param>
	void Initialize(BlockManager* blockManager, EnemyManager* enemyManager);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

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

	// スプライト
	std::unique_ptr<Sprite> sprite_;

};

