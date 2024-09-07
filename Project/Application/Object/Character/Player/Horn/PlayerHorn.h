#pragma once
#include "../../../Engine/Object/MeshObject.h"

class PlayerHorn :
	public MeshObject
{

public: // 静的メンバ関数

	/// <summary>
	/// プレイヤーの生成
	/// </summary>
	/// <param name="name">名前</param>
	/// <returns>データ</returns>
	static LevelData::MeshData PlayerHornCreate(const std::string& name);

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
	void OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData);

public:

	/// <summary>
	/// 親設定
	/// </summary>
	/// <param name="parent">親</param>
	/// <param name="parentName">ボーンの名前</param>
	void SetParent(Player* parent, const std::string& parentName);

private:

	/// <summary>
	/// 親あるとき更新
	/// </summary>
	void WithParentsUpdate();

	/// <summary>
	/// 親がない時更新
	/// </summary>
	void ParentlessUpdate();

	/// <summary>
	/// コライダーの初期化
	/// </summary>
	void ColliderInitialize();

	/// <summary>
	/// コライダーの更新
	/// </summary>
	void ColliderUpdate();

	/// <summary>
	/// ノード追従
	/// </summary>
	void NodeFollowing();

private:

	// 壊れたか
	bool broken_;

	// 親
	Player* parent_;

	// 親のノード(持っている部分)
	NodeData* parentNodeData_;

	// 親の行列
	Matrix4x4* parentMatrix_;

	// 回転
	Vector3 localRotate_ = { 0.0f, 0.0f, 0.0f };

	// 位置
	Vector3 localPosition_ = { 0.0f, 0.0f, 0.0f };

	// 大きさ
	Vector3 localScale_ = { 1.0f, 1.0f, 1.0f };

	//// 剛体
	//RigidBody rigidBody_;

	//// 反発係数
	//float coefficientOfRestitution = 0.0f;


};
