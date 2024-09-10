#pragma once
#include "../../../Engine/Object/MeshObject.h"
#include "../../../../../Engine/Animation/Animation.h"
#include "../../Player/Player.h"
#include "../BaseEnemy.h"
#include "../../../Obstacle/Block/BlockManager.h"
#include "../../../Engine/Object/BaseObjectManager.h"

class Boss;

class Hand :
	public MeshObject
{

public: // 静的メンバ関数

	/// <summary>
	/// 手の生成
	/// </summary>
	/// <returns></returns>
	static LevelData::MeshData HandCreate(size_t direction);


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
	void OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData);

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
	//void RightRoundAttack();

	/// <summary>
	/// 叩きつけ構え
	/// </summary>
	void StampStand();

	/// <summary>
	/// 叩きつけ攻撃
	/// </summary>
	void StampAttack();

	/// <summary>
	/// 被弾
	/// </summary>
	//void Damage();

public:
	void Stamp();

private: //	変数

	//hp
	int32_t hp_;

	// 初期HP
	uint32_t initHp_ = 3;

	Vector3 velocity_;

	//予備動作の終了フレーム
	size_t rushIdleLength = 60;

	//死亡アニメーションの終了フレーム
	size_t deathAnimationLength = 15;

	size_t countUp_ = 0;

	//手の向き(右:1 左:-1)
	size_t direction_=0;

	//ステート
	std::function<void(void)> state_;

	Player* target_ = nullptr;

	//被弾後の無敵時間か
	bool isHitCoolTime_ = false;

	bool isCollision_ = false;

	//スタンプ攻撃のプレイヤー追跡時間
	size_t stampChaseLength_ = 120;

	//親
	Boss* parent_ = nullptr;

public: // アクセッサ

	WorldTransform* GetWorldTransformAdress() { return &worldTransform_; }

	void SetPlayer(Player* player) { target_ = player; };

	void SetParent(Boss* parent) { parent_ = parent; };

private: // グローバル変数

	/// <summary>
	/// 調整項目の適用
	/// </summary>
	//void ApplyGlobalVariables();

	/// <summary>
	/// 調整項目の登録
	/// </summary>
	//void RegistrationGlobalVariables();

};

