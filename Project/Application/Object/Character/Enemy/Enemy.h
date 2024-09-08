#pragma once
#include "../../../Engine/Object/MeshObject.h"
#include "../../../../Engine/Animation/Animation.h"
#include "../../Obstacle/Block/BlockManager.h"
#include "../Player/Player.h"

class Enemy :
	public MeshObject
{

public: // 静的メンバ関数

	/// <summary>
	/// プレイヤーの生成
	/// </summary>
	/// <returns></returns>
	static LevelData::MeshData EnemyCreate();

	
public: // ベースのメンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(LevelData::MeshData* data) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

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

private: // ベースのメンバ変数

	

private: // ステート関数

	/// <summary>
	/// 移動状態
	/// </summary>
	void Rush();

	/// <summary>
	/// 移動開始
	/// </summary>
	void RushStart();

	/// <summary>
	/// 射撃状態
	/// </summary>
	void Shot();

	/// <summary>
	/// 射撃開始
	/// </summary>
	void ShotStart();

	/// <summary>
	/// 状態チェック
	/// </summary>
	void CheckFloorConect();

	//プレイヤーの方を向く
	void RotateToPlayer();

private: // ステート変数


private: // パーツ構成関数

	/// <summary>
	/// パーツ初期化
	/// </summary>
	void PartInitialize();

	/// <summary>
	/// コライダー更新
	/// </summary>
	void ColliderUpdate();

	/// <summary>
	/// アニメーション更新
	/// </summary>
	void AnimationUpdate();

private: // パーツ,アニメーション変数

	// 現在のモーション番号
	//uint32_t currentMotionNo_;

	// 前のモーション番号
	//uint32_t prevMotionNo_;

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
	/// ポジション修正
	/// </summary>
	void PositionLimit();

private: //	変数

	//hp
	int32_t hp_;

	// 初期HP
	uint32_t initHp_;

	// 速度
	Vector3 velocity_ = {};

	// 速度
	float runningSpeed_ = 0.3f;

	// ブロックマネージャー
	BlockManager* blockManager_ = nullptr;

	Player* target_ = nullptr;
	
	//ステート
	std::function<void(void)> state_;

public: // アクセッサ

	WorldTransform* GetWorldTransformAdress() { return &worldTransform_; }

	int32_t GetHp() { return hp_; }

	uint32_t GetInitHp() { return initHp_; }

	//LocalMatrixManager* GetLocalMatrixManager() { return localMatrixManager_.get(); }

	void SetHP(uint32_t hp) { hp_ = hp; }

	Vector3 GetVelocity() { return velocity_; }

	void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }

	float GetRunningSpeed() { return runningSpeed_; }

	void SetBlockManager(BlockManager* blockManager) { blockManager_ = blockManager; }

	BlockManager* GetBlockManager() { return blockManager_; }

	//Animation* GetAnimationAdress() { return &animation_; }

	void SetPlayer(Player* player) { target_ = player; };

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

