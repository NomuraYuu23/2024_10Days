#pragma once
#include "../../../Engine/Object/MeshObject.h"
#include "../../../../Engine/Animation/Animation.h"
#include "System/PlayerCommand.h"
#include "State/IPlayerState.h"
#include "State/PlayerStateFactory.h"
#include "../../Obstacle/Block/BlockManager.h"

/// <summary>
/// プレイヤーのモーション一覧
/// </summary>
enum PlayerMotionIndex {
	kPlayerMotionRun, // 走行時
	kPlayerMotionWait, // 通常時
	kPlayerMotionJump, // ジャンプ時
	kPlayerMotionHeadDrop, // ヘッドドロップ
	kPlayerMotionIndexOfCount // 数
};

class Player :
    public MeshObject
{

public: // 静的メンバ関数

	/// <summary>
	/// プレイヤーの生成
	/// </summary>
	/// <returns></returns>
	static LevelData::MeshData PlayerCreate();

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

	// コマンド
	PlayerCommand* playerCommand_;

	// コマンドを受け取るか
	bool receiveCommand_;

private: // ステート関数

	/// <summary>
	/// ステート初期化
	/// </summary>
	void StateInitialize();

	/// <summary>
	/// ステート更新
	/// </summary>
	void StateUpdate();

private: // ステート変数

	// ステート
	std::unique_ptr<IPlayerState> playerState_;

	// 現在のステート番号
	uint32_t currentStateNo_;

	// 前のステート番号
	uint32_t prevStateNo_;

	// 次のステート番号
	uint32_t nextStateNo_;

	// ステートファクトリー
	PlayerStateFactory* playerStateFactory_;

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
	/// ポジション修正
	/// </summary>
	void PositionLimit();

private: //	変数

	//hp
	int32_t hp_;

	// 初期HP
	uint32_t initHp_;

	// カメラ
	BaseCamera* camera_ = nullptr;

	// 速度
	Vector3 velocity_ = {};

	// 速度
	float runningSpeed_ = 0.3f;

	// ブロックマネージャー
	BlockManager* blockManager_ = nullptr;

	// ジャンプ初速
	float jumpInitialSpeed_ = 1.4f;

	// ジャンプチェックポイント
	int32_t jumpCheckpointFrame_ = 8;

	// 小ジャンプ倍率
	float smallJumpMultiplier_ = 0.5f;

	// 滞空時倍率
	float airborneMultiplier_ = 0.5f;

	// 滞空フラグ
	float airborneCheck_ = false;

public: // アクセッサ

	WorldTransform* GetWorldTransformAdress() { return &worldTransform_; }

	void SetCamera(BaseCamera* camera) { camera_ = camera; }

	BaseCamera* GetCamera() { return camera_; }

	void SetReceiveCommand(bool receiveCommand) { receiveCommand_ = receiveCommand; }

	IPlayerState* GetPlayerState() { return playerState_.get(); }

	uint32_t GetCurrentStateNo() { return currentStateNo_; }

	int32_t GetHp() { return hp_; }

	uint32_t GetInitHp() { return initHp_; }

	LocalMatrixManager* GetLocalMatrixManager() { return localMatrixManager_.get(); }

	void SetHP(uint32_t hp) { hp_ = hp; }

	Vector3 GetVelocity() { return velocity_; }
	
	void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }

	float GetRunningSpeed() { return runningSpeed_; }

	void SetBlockManager(BlockManager* blockManager) { blockManager_ = blockManager; }

	BlockManager* GetBlockManager() { return blockManager_; }

	float GetJumpInitialSpeed() { return jumpInitialSpeed_; }

	int32_t GetJumpCheckpointFrame() { return jumpCheckpointFrame_; }

	float GetSmallJumpMultiplier() { return smallJumpMultiplier_; }

	Animation* GetAnimationAdress() { return &animation_; }

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

