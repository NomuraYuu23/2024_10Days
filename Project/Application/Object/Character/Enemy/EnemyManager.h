#pragma once
#include <vector>
#include "BaseEnemy.h"
#include "../Player/Player.h"
#include "../../Obstacle/Block/BlockManager.h"

class BlockManager;
class BaseObjectManager;
class Player;

struct EnemyData {
	std::string className;
	size_t spownFrame;
	Vector3 position;
	Vector3 velocity;
};

class EnemyManager
{

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	void Update();

	/// <summary>
	/// 追加
	/// </summary>
	/// <param name="data">敵情報</param>
	void AddEnemy(EnemyData& data);

	/// <summary>
	/// 取得
	/// </summary>
	/// <returns></returns>
	//std::vector<Block*>* GetEnemys() { return &blocks_; }

	/// <summary>
	/// 敵の数
	/// </summary>
	/// <returns></returns>
	uint32_t GetEnemyNum() { return static_cast<uint32_t>(enemys_.size()); }

	void SetBlockManager(BlockManager* blockManager) { blockManager_ = blockManager; }

	BlockManager* GetBlockManager() { return blockManager_; }

	void SetObjectManager(BaseObjectManager* manager) { objectManager_ = manager; }

	void SetPlayer(Player* player) { player_ = player; };

private:

	// 
	std::list<Enemy*> enemys_;

	std::vector<std::list<EnemyData>> spownDatas_;

	size_t frameCount_=0;

	// ブロックマネージャー
	BlockManager* blockManager_ = nullptr;

	//オブジェクトマネージャー
	BaseObjectManager* objectManager_ = nullptr;

	Player* player_ = nullptr;

};

