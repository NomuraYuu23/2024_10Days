#pragma once
#include "../../../Engine/Math/Vector4.h"
#include "../../../Engine/Light/DirectionalLight/DirectionalLight.h"
class DirectionalLightSystem
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="directionalLight">平行光源</param>
	void initialize(DirectionalLight* directionalLight);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

private: // 変数

	// 一日のサイクル時間
	float dailyCycleTime_ = 60.0f;

	// 昼の色
	Vector3 daytimeColor_;

	// 夜の色
	Vector3 nightColor_;

	// タイマー
	float timer_;

	// 平行光源
	DirectionalLight* directionalLight_;

	// 平行光源データ
	DirectionalLightData directionalLightData_;

	// 昼か
	bool isDaytime_ = true;

public: // アクセッサ

	DirectionalLightData GetDirectionalLightData() { return directionalLightData_; }

	bool GetIsDaytime() { return isDaytime_; }

private: // グローバル変数

	/// <summary>
	/// 調整項目の適用
	/// </summary>
	void ApplyGlobalVariables();

	/// <summary>
	/// 調整項目の登録
	/// </summary>
	void RegistrationGlobalVariables();

};

