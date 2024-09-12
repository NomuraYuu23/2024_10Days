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
		kCountDownFlowMain, // カウントダウン
		kCountDownFlowGameStart, // ゲームスタート
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
	/// カウントダウン
	/// </summary>
	void Main();

	/// <summary>
	/// ゲーム開始
	/// </summary>
	void GameStart();

	/// <summary>
	/// エンドシステム
	/// </summary>
	void EndSystem();

private:

	// カウントMAX
	static const uint32_t kCountMax_ = 3;

	// カウント数字
	std::unique_ptr<Sprite> countNum_;
	Vector2 countNumPos_;
	Vector2 countNumSize_;
	std::array<uint32_t, kCountMax_> numTextureHandles_;

	// ゲームスタート文字
	std::unique_ptr<Sprite> gameStartString_;
	Vector2 gameStartStringPos_;
	Vector2 gameStartStringSize_;

	// 経過時間
	float elapsedTime_ = 0.0f;

	// 時間最大
	float timeMax_ = 1.0f;

	// 実行フラグ
	bool isRun_;

	// ゲーム開始フラグ
	bool isGameStart_;

	//ステート
	std::array<std::function<void(void)>, CountDownFlow::kCountDownFlowOfCount> countDownFlowUpdates_;

	// 番号
	CountDownFlow countDownFlowNumber_;

	// カウント
	uint32_t count_;

public:

	void SetIsRun(bool isRun) { isRun_ = isRun; }
	
	bool GetIsGameStart(bool isGameStart) { isGameStart_ = isGameStart; }

};

