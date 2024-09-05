#pragma once
#include "../../../Engine/3D/WorldTransform.h"
#include "../../../Engine/Camera/BaseCamera.h"

#include "../Object/Character/Player/Player.h"

class GameCamera : public BaseCamera
{

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update(float elapsedTime = 0.0f) override;

	/// <summary>
	/// プレイヤーセット
	/// </summary>
	/// <param name="player">プレイヤー</param>
	void SetPlayer(Player* player) { player_ = player; }

private: // メンバ関数

	/// <summary>
	/// 調整項目の適用
	/// </summary>
	void ApplyGlobalVariables();

	/// <summary>
	/// 調整項目の登録
	/// </summary>
	void RegistrationGlobalVariables();

private:

	// プレイヤー
	Player* player_ = nullptr;

	// ステージの中心位置
	Vector3 stageCenter_{};

	// 移動レート
	float moveRate_ = 0.2f;
	
	// 回転レート
	float rotateRate_ = 0.1f;

	// オフセット距離
	float offsetLength_ = -10.0f;

	// オフセット高さ
	float offsetHeight_ = 3.0f;

	// 方向
	Vector3 direction_ = { 0.0f,0.0f,1.0f };

};

