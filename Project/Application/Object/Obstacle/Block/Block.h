#pragma once
#include "../BaseObstacle.h"
#include "../../../../Engine/Animation/Animation.h"
class Block :
    public BaseObstacle
{

public: // パラメータ

	//一辺の数
	static const size_t kNumOnece_ = 8;

	static const float kSize_;

public: // static関数

	/// <summary>
	/// ブロックデータの生成
	/// </summary>
	/// <returns></returns>
	static LevelData::MeshData BlockCreate();

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
	void Draw(BaseCamera& camera) override;

	/// <summary>
	/// ImGui描画
	/// </summary>
	//void ImGuiDraw() override;

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="colliderPartner"></param>
	/// <param name="collisionData"></param>
	void OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData) override;

	/// <summary>
	/// コライダー更新
	/// </summary>
	void ColliderUpdate();

private: // ステート処理

	/// <summary>
	/// 待機状態
	/// </summary>
	void Idle();

	/// <summary>
	/// 移動状態
	/// </summary>
	void Move();

	/// <summary>
	/// 移動開始
	/// </summary>
	void MoveStart();

	/// <summary>
	/// 攻撃状態
	/// </summary>
	void Attack();

	/// <summary>
	/// 攻撃開始
	/// </summary>
	void AttackStart();

private: // パーツ,アニメーション変数

	// 現在のモーション番号
	uint32_t currentMotionNo_;

	// 前のモーション番号
	uint32_t prevMotionNo_;

	//ノードアニメーション
	Animation animation_;

	// ローカル行列
	std::unique_ptr<LocalMatrixManager> localMatrixManager_ = nullptr;

	//ステート
	std::function<void(void)> state_;

	//フラグ
	bool isMove_ = false;
	bool isAttack_ = false;

	//プレイヤーが触れているか
	bool isCollision_ = false;

	//高さ(f:下 t:上)
	bool hight_ = false;

	//カウント用
	size_t countUp_=0;

	//仮パラメータ
	size_t moveAnimationLength_ = 60;
	size_t attackAnimationLength_ = 15;


	//初期位置
	Vector3 initialPosition_{};

	//浮き沈みの高さ
	float floatHight_ = 16.0f;
	//攻撃時のブロックの浮き
	float attackFloatStrength_ = 3.0f;
};

