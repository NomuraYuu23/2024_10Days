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

	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw();

private: // メンバ関数

	// 追従対象からのオフセットを計算する
	Vector3 OffsetCalc() const;

	/// <summary>
	/// 調整項目の適用
	/// </summary>
	void ApplyGlobalVariables();

	/// <summary>
	/// 調整項目の登録
	/// </summary>
	void RegistrationGlobalVariables();

	/// <summary>
	/// 自動
	/// </summary>
	void Automatic();

	/// <summary>
	/// 手動
	/// </summary>
	void Manual();

	/// <summary>
	/// 設定
	/// </summary>
	void Setting();

private:

	// プレイヤー
	Player* player_ = nullptr;

	// 追従対象の残像座標
	Vector3 interTarget_ = {};

	// 目指すアングル
	float destinationAngleX_ = 0.0f;
	// 目指すアングル
	float destinationAngleY_ = 0.0f;

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

	// オフセットサイド
	float offsetSide_ = 3.0f;

	// 目指すアングルX高さ加算用最大値
	float destinationAngleXAddMax_ = 0.0f;
	
	// アングル変更用の位置下
	float fieldDown_ = -2.0f;

	// アングル変更用の位置上
	float fieldTop_ = 10.0f;

	// 目指すFOVMAX
	float targetFovYMax_ = 1.0f;
	// 目指すFOVMIN
	float targetFovYMin_ = 0.45f;

	// 目指すFOVが切り替わる位置
	float fovYChangeLine_ = 30.0f;

	// 自動モードか
	bool isAutomatic_ = true;

	// インプット
	Input* input_;

	// 手動用目指すアングルX
	float manualDestinationAngleX_;

	// 上下反転フラグ
	bool upsideDown_;

	// 左右反転フラグ
	bool leftRightFlip_;

	// スティック感度
	float rotateSpeed_ = 1.0f;
	float baseRotateSpeed_ = 0.000001f;

	// タイトル時のオフセット
	float titleCameraOffsetSide_ = 0.0f;

public:

	void SetTitleCameraOffsetSide(float titleCameraOffsetSide) { titleCameraOffsetSide_ = titleCameraOffsetSide; }

};

