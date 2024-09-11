#pragma once

#include <memory>
#include "../../../Engine/2D/Sprite.h"
#include "../../../Engine/base/DirectXCommon.h"

class GameCamera;

class TitleSystem
{

public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="dxCommon"></param>
	void Initialize(DirectXCommon* dxCommon, GameCamera* gameCamera);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:

	/// <summary>
	/// 終了システム
	/// </summary>
	void EndSystem();

private:

	/// <summary>
	/// ロゴ初期化
	/// </summary>
	/// <param name="dxCommon"></param>
	void LogoInitialize(DirectXCommon* dxCommon);

	/// <summary>
	/// ロゴ更新
	/// </summary>
	void LogoUpdate();

private:

	/// <summary>
	/// ゲームカメラ初期化
	/// </summary>
	/// <param name="gameCamera"></param>
	void GameCameraInitialize(GameCamera* gameCamera);

	/// <summary>
	/// ゲームカメラ更新
	/// </summary>
	void GameCameraUpdate();

private:

	// 実行中か
	bool isRun_;

	// 終了システム中か
	bool endSystem_;

	// 終了するまでの時間
	float endSystemTime_;

	// 終了するまでの時間 計測用
	float currentEndSystemTime_;

	// ロゴ
	std::unique_ptr<Sprite> titleLogoSprite_;
	uint32_t titleLogoTextureHandle_;

	// ゲームカメラ
	GameCamera* gameCamera_;

};

