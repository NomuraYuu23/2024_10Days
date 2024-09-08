#pragma once
#include "../../../Engine/Math/Vector4.h"
#include "../../../Engine/Light/DirectionalLight/DirectionalLight.h"
class DirectionalLightSystem
{

public: // メンバ関数

	void initialize(DirectionalLight* directionalLight);

	void Update();

private:

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

};

