#pragma once
#include "../../../Engine/Object/MeshObject.h"
#include "../../../../Engine/Animation/Animation.h"
#include "../../Obstacle/Block/BlockManager.h"
#include "../Player/Player.h"
//#include "../../../Engine/Object/BaseObjectManager.h"

class BaseObjectManager;
class EnemyManager;

class Egg :
	public MeshObject
{

public: // 静的メンバ関数

	/// <summary>
	/// プレイヤーの生成
	/// </summary>
	/// <returns></returns>
	static LevelData::MeshData EggCreate();


public: // ベースのメンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(LevelData::MeshData* data) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw(BaseCamera& camera);

	/// <summary>
	/// ImGui描画
	/// </summary>
	void ImGuiDraw() override;

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="colliderPartner"></param>
	/// <param name="collisionData"></param>
	void OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData);

	/// <summary>
	/// パーティクル描画
	/// </summary>
	/// <param name="camera">カメラ</param>
	void ParticleDraw(BaseCamera& camera) override;

	void Delete();

private: // ベースのメンバ変数



private: // ステート関数


private: // ステート変数


private: // パーツ構成関数

	/// <summary>
	/// コライダー更新
	/// </summary>
	void ColliderUpdate();

private: // 衝突処理

	/// <summary>
	/// 障害物との衝突処理
	/// </summary>
	/// <param name="colliderPartner"></param>
	/// <param name="collisionData"></param>
	void OnCollisionObstacle(ColliderParentObject colliderPartner, const CollisionData& collisionData);

private: // 関数

	void CreateEnemy();

private: //	変数

	// 速度
	Vector3 velocity_ = {};


	// ブロックマネージャー
	BlockManager* blockManager_ = nullptr;

	Player* target_ = nullptr;

	//オブジェクトマネージャー
	BaseObjectManager* objectManager_ = nullptr;

	//敵マネージャ
	EnemyManager* enemyManager_ = nullptr;

	//カウント用
	size_t countUp_ = 0;

	size_t enemyCreateTime_ = 180;

	//敵を産むか
	bool isCreateEnemy_ = true;

public: // アクセッサ

	WorldTransform* GetWorldTransformAdress() { return &worldTransform_; }

	//LocalMatrixManager* GetLocalMatrixManager() { return localMatrixManager_.get(); }

	Vector3 GetVelocity() { return velocity_; }

	void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }

	
	void SetBlockManager(BlockManager* blockManager) { blockManager_ = blockManager; }

	BlockManager* GetBlockManager() { return blockManager_; }

	void SetObjectManager(BaseObjectManager* manager) { objectManager_ = manager; }
	void SetPlayer(Player* player) { target_ = player; };

	void SetEnemyManager(EnemyManager* manager) { enemyManager_ = manager; };

	void SetIsCreateEnemy(bool is) { isCreateEnemy_ = is; };

	void SetEnemyCreateTime(size_t time) { enemyCreateTime_ = time; };

private: // グローバル変数


};

