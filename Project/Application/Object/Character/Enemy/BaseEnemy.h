#pragma once
#include "../../../Engine/Object/MeshObject.h"
#include "../../../../Engine/Animation/Animation.h"
#include "../../Obstacle/Block/BlockManager.h"
#include "../Player/Player.h"

class BaseEnemy :
	public MeshObject
{

public: // 静的メンバ関数


public: // ベースのメンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize(LevelData::MeshData* data) override {};

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() override {};

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	virtual void Draw(BaseCamera& camera) {};

	/// <summary>
	/// ImGui描画
	/// </summary>
	virtual void ImGuiDraw() override {};

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="colliderPartner"></param>
	/// <param name="collisionData"></param>
	virtual void OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData) {};

	/// <summary>
	/// パーティクル描画
	/// </summary>
	/// <param name="camera">カメラ</param>
	virtual void ParticleDraw(BaseCamera& camera) override {};

private: // パーツ構成関数

	/// <summary>
	/// コライダー更新
	/// </summary>
	virtual void ColliderUpdate() {};



private: // パーツ,アニメーション変数

private: // 衝突処理

	/// <summary>
	/// 障害物との衝突処理
	/// </summary>
	/// <param name="colliderPartner"></param>
	/// <param name="collisionData"></param>
	virtual void OnCollisionObstacle(ColliderParentObject colliderPartner, const CollisionData& collisionData) {};

private: // 関数

private: //	変数

protected:

	//フラグ
	//死亡アニメーションが再生されているかどうか、これが立った時点でenemyManagerのリストから外す
	bool isPlayDeathAnimation_ = false;


public: // アクセッサ

	bool GetIsPlayDeathAnimation_() { return isPlayDeathAnimation_; };

};

