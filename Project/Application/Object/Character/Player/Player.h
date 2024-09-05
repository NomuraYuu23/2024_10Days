#pragma once
#include "../../../Engine/Object/MeshObject.h"
#include "../../../../Engine/Animation/Animation.h"
#include "System/PlayerCommand.h"
#include "State/IPlayerState.h"
#include "State/PlayerStateFactory.h"

/// <summary>
/// プレイヤーのモーション一覧
/// </summary>
enum PlayerMotionIndex {
	kPlayerMotionWait, // 通常時
	kPlayerMotionRun, // 走行時
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
	void ImGuiDraw();

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

private: //	変数

	//hp
	int32_t hp_;

	// 初期HP
	uint32_t initHp_;

	// カメラ
	BaseCamera* camera_ = nullptr;

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

};

