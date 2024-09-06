#pragma once
#include "../../../RigidBodyObject/BaseRigidBodyObject.h"

class PlayerHorn :
	public BaseRigidBodyObject
{

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
	/// 衝突処理
	/// </summary>
	/// <param name="colliderPartner"></param>
	/// <param name="collisionData"></param>
	void OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData) override;

	/// <summary>
	/// パーティクル更新
	/// </summary>
	void ParticleUpdate();

	/// <summary>
	/// パーティクル描画
	/// </summary>
	/// <param name="camera">カメラ</param>
	void ParticleDraw(BaseCamera& camera) override;

public:

	/// <summary>
	/// 親設定
	/// </summary>
	/// <param name="parent">親</param>
	void SetParent(Player* parent);

private:

	// 壊れたか
	bool broken_;

	// 親
	Player* parent_;

	// 親のノード(持っている部分)
	NodeData* parentNodeData_;

	// 親の行列
	Matrix4x4* parentMatrix_;

};
