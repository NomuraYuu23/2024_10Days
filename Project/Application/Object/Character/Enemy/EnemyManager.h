#pragma once
#include <vector>
#include "BaseEnemy.h"
#include "../Player/Player.h"
#include "../../Obstacle/Block/BlockManager.h"
#include "EggBreakParticleManager.h"

class BlockManager;
class BaseObjectManager;
class Player;
class Egg;

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

	void PostUpdate();

	void ParticleDraw(BaseCamera& camera);

	/// <summary>
	/// 追加
	/// </summary>
	/// <param name="data">敵情報</param>
	void AddEnemy(EnemyData& data);

	//タマゴが敵を生成したときにも入れる用
	void AddEnemy(BaseEnemy* enemy);

	/// <summary>
	/// 卵生成
	/// </summary>
	/// <param name="position">位置</param>
	void AddEgg(const Vector3& position);

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

	std::list<BaseEnemy*>* GetEnemys() { return &enemys_; }
	std::list<BaseEnemy*>* GetFlyEnemys() { return &flyEnemys_; }
	std::list<Egg*>* GetEggs() { return &eggs_; }

	void SetAudioManager(GameAudioManager* audioManager) { audioManager_ = audioManager; }
	GameAudioManager* GetAudioManager() { return audioManager_; }

	//入ってきたポインタをリストから排除する
	void RemoveEgg(Egg* in);

	bool GetIsEndAllWave() { return isEndAllWave_ && isBossSpawn_; };

private:

	// 
	std::list<BaseEnemy*> enemys_;

	std::list<BaseEnemy*> flyEnemys_;

	std::list<Egg*> eggs_;

	std::vector<std::list<EnemyData>> spownDatas_;

	size_t frameCount_=0;

	//ウェーブ数(仮)
	size_t kWaveNum = 5;

	size_t waveNum = 0;

	//すべてのウェーブが終了したか
	bool isEndAllWave_ = false;

	bool isBossSpawn_ = false;

	//ウェーブ終了からボスが出現するまでの時間
	size_t bossSpwanTime_ = 180;

	// ブロックマネージャー
	BlockManager* blockManager_ = nullptr;

	//オブジェクトマネージャー
	BaseObjectManager* objectManager_ = nullptr;

	Player* player_ = nullptr;

	// 卵割れるエフェクト
	std::unique_ptr<EggBreakParticleManager> eggBreakParticleManager_;

	GameAudioManager* audioManager_;

};

