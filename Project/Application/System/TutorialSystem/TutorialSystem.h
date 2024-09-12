#pragma once
#include "../../Object/Manager/GameSceneObjectManager.h"
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
		kTutorialFlowOfCount, // 数
	};

	// 開始チェック
	struct StartCheckStruct
	{
		bool isClear_; // 達成した

		Vector3 center_; // 中心座標

		float radius_; // 半径

	};

	// ジャンプチェック
	struct StartCheckStruct
	{
		bool isClear_; // 達成した

		float checkTime_; // アニメーション時間の更新が始まるタイミング

	};

public: 

	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

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

private: // 変数

	// オブジェクトマネージャー
	GameSceneObjectManager* gameSceneObjectManager_;

	// プレイヤー
	Player* player_;



	StartCheckStruct startCheckStruct_;


};

