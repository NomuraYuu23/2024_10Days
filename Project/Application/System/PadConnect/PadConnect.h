#pragma once
#include <memory>
#include "../../../Engine/2D/Sprite.h"
#include "../../../Engine/Input/Input.h"

class PadConnect
{

public:

	void Initialize();

	void Update();

	void Draw();

	void SetIsRun(bool isRun) { isRun_ = isRun; }

private:

	std::unique_ptr<Sprite> sprite_;

	Vector2 pos_;

	Vector2 size_;

	Input* input_;

	bool isRun_;

};

