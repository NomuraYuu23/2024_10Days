#include "directionalLightSystem.h"
#include "../../../Engine/Math/DeltaTime.h"
#include "../../../Engine/Math/Ease.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"

void DirectionalLightSystem::initialize(DirectionalLight* directionalLight)
{

	// 一日のサイクル時間
	dailyCycleTime_ = 60.0f;

	// 昼の色
	daytimeColor_ = { 235.0f / 255.0f, 234.0f / 255.0f, 177.0f / 255.0f };

	// 夜の色
	nightColor_ = { 0.1f, 0.3f, 0.5f };

	// タイマー
	timer_ = 0.0f;

	// 平行光源
	directionalLight_ = directionalLight;

	// 平行光源データ
	directionalLightData_.color = { daytimeColor_.x,daytimeColor_.y,daytimeColor_.z,1.0f };
	directionalLightData_.direction = { 0.0f, -1.0f, 0.0f };
	directionalLightData_.intencity = 1.0f;

	// システム開始フラグ
	isStart_ = false;

	// カウントダウン中
	countDownNow_ = false;

	RegistrationGlobalVariables();
	ApplyGlobalVariables();

	// カウントダウンスピード
	countDownSpeed_ = dailyCycleTime_ / 60.0f;

}

void DirectionalLightSystem::Update()
{

	if (!isStart_) {
		return;
	}

	// タイマーを進める
	if (countDownNow_) {
		timer_ = fmodf(timer_ + countDownSpeed_, dailyCycleTime_);
	}
	else {
		timer_ = fmodf(timer_ + kDeltaTime_, dailyCycleTime_);
	}

	// 色の変更
	Vector3 color{};
	float t = timer_ / dailyCycleTime_;
	bool isDaytime = true;
	// 昼から夜
	if (t >= 0.25f && t < 0.75f) {
		t -= 0.25f;
		t *= 2.0f;
	}
	// 夜から昼
	else {
		isDaytime = false;
		if (t >= 0.75f) {
			t -= 0.75f;
			t *= 2.0f;
		}
		else {
			t += 0.25f;
			t *= 2.0f;
		}
	}

	if (isDaytime) {
		color = Ease::Easing(Ease::EaseName::EaseOutQuart, daytimeColor_, nightColor_, t);
	}
	else {
		color = Ease::Easing(Ease::EaseName::EaseOutQuart, nightColor_, daytimeColor_, t);
	}

	// 方向
	Vector3 direction = { 0.0f, -1.0f, 0.0f};

	// 強さの変更

	// データを更新
	directionalLightData_.color = { color.x, color.y, color.z, 1.0f };
	directionalLightData_.direction = direction;
	directionalLightData_.intencity = 1.0f;
	directionalLight_->Update(directionalLightData_);

}

void DirectionalLightSystem::ApplyGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "DirectionalLight";

	dailyCycleTime_ = globalVariables->GetFloatValue(groupName, "dailyCycleTime");
	daytimeColor_ = globalVariables->GetVector3Value(groupName, "daytimeColor");
	nightColor_ = globalVariables->GetVector3Value(groupName, "nightColor");

}

void DirectionalLightSystem::RegistrationGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "DirectionalLight";

	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "dailyCycleTime", dailyCycleTime_);
	globalVariables->AddItem(groupName, "daytimeColor", daytimeColor_);
	globalVariables->AddItem(groupName, "nightColor", nightColor_);

}
