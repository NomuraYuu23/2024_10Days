#pragma once
#include "../../../Engine/Math/Vector3.h"
#include "../../../Engine/Object/MeshObject.h"
#include "TutorialArrowObject.h"
#include "../../../Engine/2D/Sprite.h"

class BaseObjectManager;

class Player;

class BlockManager;

class GameCamera;

class TutorialSystem
{

	/// <summary>
	/// チュートリアルの流れ
	/// </summary>
	enum TutorialFlow {
		kTutorialFlowStartCheck, // 開始チェック
		kTutorialFlowJumpCheck, // ジャンプチェック
		kTutorialFlowUpperRowOccurrence, // 上段オブジェクト発生
		kTutorialFlowKnockFromBelowCheck, // 下からたたくチェック
		kTutorialFlowLowerRowOccurrence, // 下段オブジェクト発生
		kTutorialFlowFallingAttackCheck, // 落下攻撃チェック
		kTutorialFlowEndSystem, // エンドシステム
		kTutorialFlowOfCount, // 数
	};

	// 開始チェック
	struct StartCheckStruct
	{

		Vector3 center_; // 中心座標
		float radius_; // 半径

	};

	// ジャンプチェック
	struct JumpCheckStruct
	{
		bool isSmallJumpClear_; // 達成した
		bool isJumpClear_; // 達成した

	};

public: 

	void Initialize(BaseObjectManager* objectManager, Player* player, BlockManager* blockManager, GameCamera* gameCamera);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(BaseCamera& camera);

	/// <summary>
	/// スプライト描画
	/// </summary>
	void SpriteDraw();

private: // 確認関数

	/// <summary>
	/// ジャンプチェック
	/// </summary>
	void JumpCheck();

	/// <summary>
	/// 下からたたくチェック
	/// </summary>
	void KnockFromBelowCheck();

	/// <summary>
	/// 落下攻撃チェック
	/// </summary>
	void FallingAttackCheck();

private: // オブジェクト発生

	/// <summary>
	/// 上段オブジェクト発生
	/// </summary>
	void UpperRowOccurrence();

	/// <summary>
	/// 下段オブジェクト発生
	/// </summary>
	void LowerRowOccurrence();

private: // その他のながれ

	/// <summary>
	/// 開始チェック
	/// </summary>
	void StartCheck();

	/// <summary>
	/// 終了システム
	/// </summary>
	void EndSystem();

private: // オブジェクト初期化

	void StartPosObjectInitialize();

	void TutorialArrowObjectInitialize();

	void SpriteInitialize();

private: // 変数

	// オブジェクトマネージャー
	BaseObjectManager* objectManager_;

	// プレイヤー
	Player* player_;

	// ブロックマネージャー
	BlockManager* blockManager_;

	// ゲームカメラ
	GameCamera* gameCamera_;

	// チュートリアル流れ
	TutorialFlow tutorialFlowNumber_;

	//ステート
	std::array<std::function<void(void)>, TutorialFlow::kTutorialFlowOfCount> tutorialFlowUpdates_;

	// 構造体たち
	StartCheckStruct startCheckStruct_;
	JumpCheckStruct jumpCheckStruct_;

	// スタート位置オブジェクト
	std::unique_ptr<MeshObject> startPosObject_;

	// スタート位置矢印オブジェクト
	std::unique_ptr<TutorialArrowObject> tutorialArrowObject_;

	// ジャンプスプライト1
	std::unique_ptr<Sprite> jumpSprite1_;
	// ジャンプスプライト2
	std::unique_ptr<Sprite> jumpSprite2_;
	// 攻撃１スプライト
	std::unique_ptr<Sprite> attack1Sprite_;
	// 攻撃２スプライト
	std::unique_ptr<Sprite> attack2Sprite_;

	// 
	float drawPosX_ = 300.0f;

	float notDrawPosX_ = -640.0f;

	float posY_ = 135.0f;

	float elapsedTime_ = 0.0f;

	float timeMax_ = 1.0f;

	bool isEndFlow_;
	
};

