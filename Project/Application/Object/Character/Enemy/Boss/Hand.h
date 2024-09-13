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
	static LevelData::MeshData HandCreate(int32_t direction);


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
	/// 薙ぎ払い攻撃構え
	/// </summary>
	void RoundStand();

	/// <summary>
	/// 薙ぎ払い攻撃
	/// </summary>
	void RoundAttack();

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
	void Damage();

	/// <summary>
	/// 死亡
	/// </summary>
	void Dead();

	//攻撃受けた時のあれこれ
	void Hit();

public:
	//本体からの命令

	//叩きつけ
	void Stamp();

	//薙ぎ払い
	void Round();

	void ConnectJoint(WorldTransform * pointer);

private: //	変数

	//hp
	int32_t hp_;

	// 初期HP
	uint32_t initHp_ = 3;

	Vector3 velocity_;

	//予備動作の終了フレーム
	size_t rushIdleLength = 60;

	//死亡アニメーションの終了フレーム
	size_t deathAnimationLength = 120;

	size_t countUp_ = 0;

	//手の向き(右:1 左:-1)
	float direction_=0;

	//ステート
	std::function<void(void)> state_;

	Player* target_ = nullptr;

	//被弾後の無敵時間か
	bool isHitCoolTime_ = false;

	bool isCollision_ = false;

	//スタンプ攻撃のプレイヤー追跡時間
	size_t stampChaseLength_ = 120;

	//被弾アニメーションの終了フレーム
	size_t damageAnimationLength = 30;

	static const size_t kRoundAnimationLength_ = 180;

	//たたきつけの硬直時間
	size_t stampStiffnessLength = 240;

	//薙ぎ払い攻撃の移動幅
	float roundAttackWidth_ = 56.0f;

	//親
	Boss* parent_ = nullptr;

	//物理的に障害物と当たるか
	bool isCollisionObstacle_ = false;

	Vector3 acceleration_;

	//攻撃判定があるか
	bool isAttack_ = false;

	//動いてるブロックでダメージを受けるか
	bool isDamageMovingBlock_ = false;

public: // アクセッサ

	WorldTransform* GetWorldTransformAdress() { return &worldTransform_; }

	void SetPlayer(Player* player) { target_ = player; };

	void SetParent(Boss* parent) { parent_ = parent; };

	bool IsAttack() { return isAttack_; };

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

