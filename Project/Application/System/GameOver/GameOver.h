#pragma once
#include "../../../Engine/2D/Sprite.h"
class GameOver
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

	std::unique_ptr<Sprite> gameoverSprite_;
	std::unique_ptr<Sprite> goTitleSprite_;
	std::unique_ptr<Sprite> buttonSprite_;
	std::unique_ptr<Sprite> blockSprite_;

};

