#pragma once
#include "../../../Engine/Object/MeshObject.h"
#include "../../../../../Engine/Animation/Animation.h"
#include "../../Player/Player.h"
#include "../BaseEnemy.h"
#include "../../../Obstacle/Block/BlockManager.h"
#include "../../../Engine/Object/BaseObjectManager.h"
#include "Hand.h"
#include "Head.h"
#include "../EnemyManager.h"
class Boss :
	public MeshObject
{

public: // 静的メンバ関数

	/// <summary>
	/// 弾の生成
	/// </summary>
	/// <returns></returns>
	static LevelData::MeshData BossCreate();


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
	void ImGuiDraw() override {};

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="colliderPartner"></param>
	/// <param name="collisionData"></param>
	void OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData) ;

	/// <summary>
	/// パーティクル描画
	/// </summary>
	/// <param name="camera">カメラ</param>
	void ParticleDraw(BaseCamera& camera) override;

private: // パーツ構成関数

	/// <summary>
	/// コライダー更新
	/// </summary>
	void ColliderUpdate();


private: // パーツ,アニメーション変数

	// 現在のモーション番号
	uint32_t currentMotionNo_;

	// 前のモーション番号
	uint32_t prevMotionNo_;

	//ノードアニメーション
	//Animation animation_;

	// ローカル行列
	std::unique_ptr<LocalMatrixManager> localMatrixManager_ = nullptr;

private: // 衝突処理

	/// <summary>
	/// 障害物との衝突処理
	/// </summary>
	/// <param name="colliderPartner"></param>
	/// <param name="collisionData"></param>
	void OnCollisionObstacle(ColliderParentObject colliderPartner, const CollisionData& collisionData);

private: // 関数


private: // ステート関数

	/// <summary>
	/// 待機
	/// </summary>
	void Root();

	/// <summary>
	/// 回転攻撃
	/// </summary>
	void RightRoundAttack();
	void LeftRoundAttack();

	/// <summary>
	/// 叩きつけ攻撃
	/// </summary>
	void RightStampAttack();
	void LeftStampAttack();

	//頭突き攻撃
	void HeadButtAttack();

	/// <summary>
	/// ダメージ
	/// </summary>
	void Damage();


	//出現
	void Spawn();

	/// <summary>
	/// 死亡状態
	/// </summary>
	void Dead();

	/// <summary>
	/// 雑魚敵召喚
	/// </summary>
	void Summon();

	void SummonPhaseOne();
	void SummonPhaseTwo();
	void SummonPhaseThree();

public:
	void CreateHand();

	void CreateHead();

	//攻撃が終了したときに子が実行する
	void EndAttack();

	void EndHeadAttack();

	void DeathRightHand();

	void DeathLeftHand();

	void DamageHead();

	void DeathHead();

	void RotateToPlayer();

	//プレイヤーの高さに移動する
	void ChacePlayerY();

private: //	変数

	//予備動作の終了フレーム
	size_t rushIdleLength = 60;

	//死亡アニメーションの終了フレーム
	size_t deathAnimationLength = 15;

	size_t countUp_=0;

	//ステート
	std::function<void(void)> state_;

	WorldTransform rightArmJointWorldTransform_;
	WorldTransform leftArmJointWorldTransform_;

	WorldTransform headJointWorldTransform_;

	WorldTransform bodyJointWorldTransform_;

	// ブロックマネージャー
	BlockManager* blockManager_ = nullptr;

	//オブジェクトマネージャー
	BaseObjectManager* objectManager_ = nullptr;

	Player* target_ = nullptr;

	Hand* rightHand_ = nullptr;
	Hand* leftHand_ = nullptr;

	Head* head_ = nullptr;

	//薙ぎ払いの準備時間
	static const size_t kRightHandRoundMoveLength_ = 60;

	//仮行動制御
	int32_t executeAction_ = 1;

	//頭突きが連続で出る確率、0で交互、1で頭突きしかしなくなる
	float headButtProbability_ = 0.7f;

	size_t headButtMoveLength_ = 60;

	//ダメージ受けた時に下に沈むまでの時間
	size_t damageAnimationlength_ = 60;

	//出現の長さ
	size_t spawnAnimationLength_ = 120;

	size_t summonAnimationLength_ = 180;

	float moveTargetY_;
	float moveFromY_;

	EnemyManager* enemyManager_;

public: // アクセッサ

	WorldTransform* GetWorldTransformAdress() { return &worldTransform_; }

	void SetBlockManager(BlockManager* blockManager) { blockManager_ = blockManager; }

	BlockManager* GetBlockManager() { return blockManager_; }

	void SetObjectManager(BaseObjectManager* manager) { objectManager_ = manager; }

	//Animation* GetAnimationAdress() { return &animation_; }

	void SetPlayer(Player* player) { target_ = player; };

	void SetEnemyManager(EnemyManager* manager) { enemyManager_ = manager; };

private: // グローバル変数

	/// <summary>
	/// 調整項目の適用
	/// </summary>
	//void ApplyGlobalVariables();

	/// <summary>
	/// 調整項目の登録
	/// </summary>
	//void RegistrationGlobalVariables();


private: //各動作の腕の位置

	Vector3 rightHandInitPos_ = { 0.0f,-32.0f,0.0f };
	Vector3 rightHandRootPos_ = {8.0f,0.0f,2.0f} ;
	Vector3 rightHandRoundPos_ = {24.0f,4.0f,0.0f} ;

	Vector3 leftHandInitPos_ = { 0.0f,-32.0f,0.0f };
	Vector3 leftHandRootPos_ = { -8.0f,0.0f,2.0f };
	Vector3 leftHandRoundPos_ = { -24.0f,4.0f,0.0f };

	Vector3 HeadInitPos_ = { 0.0f,9.0f,0.0f };

	Vector3 HeadAttackPos_ = { 0.0f,4.0f,4.0f };

	Vector3 oridinSpownPos_ = {0.0f,-64.0f,0.0f};
	Vector3 oridinRootPos_ = { 0.0f,0.0f,32.0f };

	Vector3 bodyRootPos_ = {0,0,0};
	Vector3 bodyHeadButtPos_ = {0,2.0f,-8.0f};
	Vector3 bodyHeadButRot_ = {3.141592f * 0.5f,0.0f,0.0f};
};

