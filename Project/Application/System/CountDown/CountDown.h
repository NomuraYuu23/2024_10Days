#pragma once
#include <memory>
#include "../../../Engine/2D/Sprite.h"

class CountDown
{

public:

	/// <summary>
	/// チュートリアルの流れ
	/// </summary>
	enum CountDownFlow {
		kCountDownFlowCome, // 来る
		kCountDownFlowMain, // カウントダウン
		kCountDownFlowGameStart, // ゲームスタート
		kCountDownFlowGoHome, // 帰る
		kCountDownFlowEndSystem, // エンドシステム
		kCountDownFlowOfCount, // 数
	};

public:

	void Initialize();

	void Update();

	void Draw();

private:

	/// <summary>
	/// スプライト
	/// </summary>
	void SpriteInitialize();

private:

	/// <summary>
	/// 来る
	/// </summary>
	void Come();

	/// <summary>
	/// カウントダウン
	/// </summary>
	void Main();

	/// <summary>
	/// ゲーム開始
	/// </summary>
	void GameStart();

	/// <summary>
	/// 帰る
	/// </summary>
	void GoHome();

	/// <summary>
	/// エンドシステム
	/// </summary>
	void EndSystem();

private:

	// カウント数字
	std::unique_ptr<Sprite> countNum_;
	Vector2 countNumPos_;
	Vector2 countNumSize_;
	std::array<uint32_t, 3> numTextureHandles_;

	// ゲームスタート文字
	std::unique_ptr<Sprite> gameStartString_;
	Vector2 gameStartStringPos_;
	Vector2 gameStartStringSize_;

	// 描画しない時の位置Y
	float notDrawPosY_ = -640.0f;

	// 経過時間
	float elapsedTime_ = 0.0f;

	// 時間最大
	float timeMax_ = 1.0f;

	// 流れが終了した
	bool isEndFlow_ = false;

	// 実行フラグ
	bool isRun_;

	// 終了フラグ
	bool isEnd_;

	// ゲーム開始フラグ
	bool isGameStart_;

	//ステート
	std::array<std::function<void(void)>, CountDownFlow::kCountDownFlowOfCount> countDownFlowUpdates_;

	// 番号
	CountDownFlow countDownFlowNumber_;

};

