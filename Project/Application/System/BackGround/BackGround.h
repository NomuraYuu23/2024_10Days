#pragma once
#include "../../../Engine/2D/Sprite.h"
class BackGround
{

public:
	
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="textureHandle"></param>
	void Initialize(uint32_t textureHandle);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="color"></param>
	void Update(const Vector4& color);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:

	// スプライト
	Sprite* sprite_;

};

