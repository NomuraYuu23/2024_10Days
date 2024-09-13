#pragma once
#include "../../../Engine/2D/Sprite.h"
class GameClear
{

public:

	void Initialize();

	void Update(bool isGameOver);

	void Draw();

	bool GetIsEnd() { return isEnd_; }

	bool GetIsRun() { return isRun_; }

private:

	// 実行
	bool isRun_;

	// 終了
	bool isEnd_;

	// 猶予時間
	float graceTime_ = 1.0f;

	// 猶予タイマー
	float graceTimer_ = 0.0f;

	std::unique_ptr<Sprite> gameclearSprite_;
	std::unique_ptr<Sprite> goTitleSprite_;
	std::unique_ptr<Sprite> buttonSprite_;
	std::unique_ptr<Sprite> blockSprite_;

};

