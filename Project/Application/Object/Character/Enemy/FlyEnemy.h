#pragma once
#include "../../../Engine/Object/MeshObject.h"
#include "../../../../Engine/Animation/Animation.h"
#include "../../Obstacle/Block/BlockManager.h"
#include "../Player/Player.h"
#include "BaseEnemy.h"

/// <summary>
/// エネミーのモーション一覧
/// </summary>
enum FlyEnemyMotionIndex {
	kFlyEnemyMotionMove, // 移動時
	kFlyEnemyMotionAttackIdle, // 攻撃予備動作時
	kFlyEnemyMotionAttack, // 攻撃時
	kFlyEnemyMotionDead, // 死亡時
	kFlyEnemyMotionIndexOfCount // 数
};

class FlyEnemy :
	public BaseEnemy
{

public: // 静的メンバ関数

	/// <summary>
	/// 弾の生成
	/// </summary>
	/// <returns></returns>
	static LevelData::MeshData FlyEnemyCreate();


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

	/// <summary>
	/// パーツ初期化
	/// </summary>
	void PartInitialize();

	/// <summary>
	/// アニメーション更新
	/// </summary>
	void AnimationUpdate();

private: // パーツ,アニメーション変数

	// 現在のモーション番号
	uint32_t currentMotionNo_;

	// 前のモーション番号
	uint32_t prevMotionNo_;

	//ノードアニメーション
	Animation animation_;

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

	/// <summary>
	/// 範囲外削除
	/// </summary>
	void PositionLimit();

	// 攻撃範囲内か
	bool IsInnerAttackArea();

private: // ステート関数

	/// <summary>
	/// 通常移動状態
	/// </summary>
	void Move();

	/// <summary>
	/// 回転予備動作
	/// </summary>
	void RushIdle();

	/// <summary>
	/// 回転突進状態
	/// </summary>
	void Rush();

	/// <summary>
	/// 死亡状態
	/// </summary>
	void Dead();

private: //	変数

	// 速度
	Vector3 velocity_ = {};

	// 速度
	float speed_ = 0.3f;

	//攻撃時の速度
	float attackSpeed_ = 0.5f;

	//予備動作の終了フレーム
	size_t rushIdleLength = 60;

	//死亡アニメーションの終了フレーム
	size_t deathAnimationLength = 15;

	size_t countUp=0;

	//ステート
	std::function<void(void)> state_;

public: // アクセッサ

	WorldTransform* GetWorldTransformAdress() { return &worldTransform_; }

	Vector3 GetVelocity() { return velocity_; }

	void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }

private: // グローバル変数

	/// <summary>
	/// 調整項目の適用
	/// </summary>
	void ApplyGlobalVariables();

	/// <summary>
	/// 調整項目の登録
	/// </summary>
	void RegistrationGlobalVariables();

};

