#pragma once
#include "../../../Engine/Object/MeshObject.h"
#include "../../../../../Engine/Animation/Animation.h"
#include "../../Player/Player.h"
#include "../BaseEnemy.h"
#include "../../../Obstacle/Block/BlockManager.h"
#include "../../../Engine/Object/BaseObjectManager.h"
#include "AttackArrow.h"
class Boss;


/// <summary>
/// 頭のモーション一覧
/// </summary>
enum HeadMotionIndex {
	kHeadMotionNormal, // 通常時
	kHeadMotionRoar, // 噛みつき時
	kHeadMotionIndexOfCount // 数
};

class Head :
	public MeshObject
{

public: // 静的メンバ関数

	/// <summary>
	/// 頭の生成
	/// </summary>
	/// <returns></returns>
	static LevelData::MeshData HeadCreate();


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


private: // ステート関数

	/// <summary>
	/// 待機
	/// </summary>
	void Root();

	/// <summary>
	/// 頭突き
	/// </summary>
	void Attack();

	//攻撃後の引き戻し
	void PullBack();

	/// <summary>
	/// 被弾
	/// </summary>
	void Damage();

	/// <summary>
	/// 咆哮
	/// </summary>
	void Roar();

	/// <summary>
	/// 死亡
	/// </summary>
	void Dead();

	void DisConnect();

	//攻撃受けた時のあれこれ
	void Hit();

public:
	//本体からの命令

	//頭突き
	void AttackCall();

	//薙ぎ払い
	//void Round();

	//召喚
	void Summon();
	void SummonEnd();

	void ConnectJoint(WorldTransform* pointer);

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

	//頭突きの全体フレーム
	static const size_t kAttackAnimationLength_ = 180;

	//頭突きの移動完了までのフレーム
	static const size_t kAttackMoveLength_ = 90;

	//引き戻しの全体フレーム
	static const size_t kPullBackLength_ = 120;

	//攻撃の移動幅
	float attackWidth_ = 44.0f;

	//ステート
	std::function<void(void)> state_;

	Player* target_ = nullptr;

	// カメラ
	BaseCamera* camera_ = nullptr;

	//被弾後の無敵時間か
	bool isHitCoolTime_ = false;

	bool isCollision_ = false;

	//被弾アニメーションの終了フレーム
	size_t damageAnimationLength = 120;

	//死亡アニメーションの終了フレーム
	size_t deadAnimationLength = 120;

	//親
	Boss* parent_ = nullptr;

	//物理的に障害物と当たるか
	bool isCollisionObstacle_ = false;

	Vector3 acceleration_;

	//攻撃判定があるか
	bool isAttack_ = false;

	//動いてるブロックでダメージを受けるか
	bool isDamageMovingBlock_ = false;

	Vector3 roarRotate_ = {-0.5f,0.0f,0.0f};

	//クリア演出用の保存座標
	Vector3 clearStartPos_;
	Vector3 clearMiddlePos_;

	std::unique_ptr<AttackArrowObject> arrow_;

public: // アクセッサ

	WorldTransform* GetWorldTransformAdress() { return &worldTransform_; }

	void SetPlayer(Player* player) { target_ = player; };

	void SetParent(Boss* parent) { parent_ = parent; };

	bool IsAttack() { return isAttack_; };

	int32_t GetHp() { return hp_; };

	void SetCamera(BaseCamera* camera) { camera_ = camera; }

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

