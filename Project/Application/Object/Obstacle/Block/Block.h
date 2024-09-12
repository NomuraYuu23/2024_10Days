#pragma once
#include "../BaseObstacle.h"
#include "../../../../Engine/Animation/Animation.h"
#include "../../../AudioManager/GameAudioManager.h"

class ShockWave
{
public:
	ShockWave() {};
	~ShockWave() {};
	void OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData) {};
private:
};

class Block :
    public BaseObstacle
{

public: // パラメータ

	//一辺の数
	static const size_t kNumOnece_ = 6;

	static const float kSize_;

	//浮き沈みの高さ
	static const float kFloatHight;

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
	/// コライダー登録
	/// </summary>
	/// <param name="collisionManager"></param>
	void CollisionListRegister(CollisionManager* collisionManager) override;

	/// <summary>
	/// コライダー登録
	/// </summary>
	/// <param name="collisionManager"></param>
	void CollisionListRegister(CollisionManager* collisionManager, ColliderDebugDraw* colliderDebugDraw) override;

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

public: //衝撃波用インナークラス

	std::unique_ptr<ShockWave> shockWave_;

private: // ステート処理

	/// <summary>
	/// 衝撃波用コライダー初期化
	/// </summary>
	void ShockWaveColliderInitialize();

	/// <summary>
	/// 衝撃波用コライダー更新
	/// </summary>
	void ShockWaveColliderUpdate();

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

	/// <summary>
	/// 衝撃波(中心)
	/// </summary>
	void ShockWaveCenter();

	/// <summary>
	/// 衝撃波(外側)
	/// </summary>
	void ShockWaveAfter();

	//ブロックを押し上げる
	void Up();

	//ブロックを下げる
	void Down();

public: //アクセッサ
	bool GetHight() { return hight_; };

	bool GetIsMove() { return isMove_; };

	//攻撃判定が出ているか
	bool GetIsAttack() { return isAttack_ || isShockWave_; };

	void SetAudioManager(GameAudioManager* audioManager) { audioManager_ = audioManager; }

	bool GetIsMoveNow() { return isMoveNow_; };

	void SetIsRockMove(bool is) { isRockMove_ = is; };

	bool GetIsRockMove() { return isRockMove_; };

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

	//衝撃波用コライダー
	std::unique_ptr<ColliderShape> shockWaveCollider_;

	//フラグ
	bool isMove_ = false;
	bool isAttack_ = false;
	bool isShockWave_ = false;
	bool isMoveNow_ = false;

	//チュートリアル用
	bool isRockMove_ = false;//移動を禁止するか


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
	float floatHight_ = kFloatHight;
	//攻撃時のブロックの浮き
	float attackFloatStrength_ = 3.0f;

	//枯れた色
	Vector3 blownColor_ = {200.0f/255.0f,160.0f/255.0f,300.0f/255.0f};

	//色の変化の長さ
	static const size_t colorLength_ = 180;
	//色の変化
	size_t colorCount_ = 0;

	//衝撃波用判定を出すか
	bool isShockWaveCollision_ = false;

	GameAudioManager* audioManager_;

};

