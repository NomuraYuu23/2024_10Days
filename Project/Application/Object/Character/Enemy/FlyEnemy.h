#pragma once
#include "../../../Engine/Object/MeshObject.h"
#include "../../../../Engine/Animation/Animation.h"
#include "../../Obstacle/Block/BlockManager.h"
#include "../Player/Player.h"

class FlyEnemy :
	public MeshObject
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



private: // パーツ,アニメーション変数

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

private: //	変数

	// 速度
	Vector3 velocity_ = {};

	// 速度
	float speed_ = 0.3f;

public: // アクセッサ

	WorldTransform* GetWorldTransformAdress() { return &worldTransform_; }

	Vector3 GetVelocity() { return velocity_; }

	void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }

};

