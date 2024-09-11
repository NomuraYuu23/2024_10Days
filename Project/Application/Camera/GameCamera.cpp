#include "GameCamera.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../../Engine/Math/Math.h"
#include "../../../Engine/Math/Ease.h"
#include "../../Engine/Math/DeltaTime.h"
#include "../../externals/imgui/imgui.h"

void GameCamera::Initialize()
{

	BaseCamera::Initialize();

	// プレイヤー
	player_ = nullptr;


	// 追従対象の残像座標
	interTarget_ = { 0.0f, 0.0f, 0.0f };

	// 目指すアングル
	destinationAngleX_ = 3.14f / 4.0f;
	// 目指すアングル
	destinationAngleY_ = 0.0f;

	// ステージの中心位置
	stageCenter_ = { 0.0f, 0.0f, 0.0f };

	// 移動レート
	moveRate_ = 0.2f;

	// 回転レート
	rotateRate_ = 0.1f;

	// オフセット距離
	offsetLength_ = -70.0f;

	// オフセット高さ
	offsetHeight_ = 3.0f;

	// オフセットサイド
	offsetSide_ = 3.0f;

	// 目指すアングルX高さ加算用最大値
	destinationAngleXAddMax_ = 0.1f;

	// アングル変更用の位置下
	fieldDown_ = -2.0f;

	// アングル変更用の位置上
	fieldTop_ = 10.0f;

	// 目指すFOVMAX
	targetFovYMax_ = 1.0f;
	// 目指すFOVMIN
	targetFovYMin_ = 0.45f;

	// 目指すFOVが切り替わる位置
	fovYChangeLine_ = 30.0f;

	isAutomatic_ = true;

	manualDestinationAngleX_ = 0.0f;

	input_ = Input::GetInstance();

	upsideDown_ = false;
	leftRightFlip_ = false;

	RegistrationGlobalVariables();
	ApplyGlobalVariables();

}

void GameCamera::Update(float elapsedTime)
{

	assert(player_);

#ifdef _DEMO
	ApplyGlobalVariables();

	// セッティング
	Setting();

#endif // _DEMO

	// モード
	if (input_->TriggerJoystick(JoystickButton::kJoystickButtonRST)) {
		if (isAutomatic_) {
			isAutomatic_ = false;
		}
		else {
			isAutomatic_ = true;
		}
	}
	else if ((Vector2::Length(input_->GetRightAnalogstick()) != 0.0f) && isAutomatic_) {
		isAutomatic_ = false;
	}

	if (isAutomatic_) {
		Automatic();
	}
	else {
		Manual();
	}

	// 追従対象がいれば
	// 追従座標の補間
	interTarget_ = Ease::Easing(Ease::EaseName::Lerp, interTarget_, stageCenter_, moveRate_);

	// オフセット
	Vector3 offset = OffsetCalc();

	transform_.translate = Vector3::Add(interTarget_, offset);

	// FOV
	if (player_->GetCurrentStateNo() == kPlayerStateHeadDrop) {
		targetFovY_ = targetFovYMax_;
	}
	else {
		targetFovY_ = targetFovYMin_;
	}


	//ビュー更新
	BaseCamera::Update(kDeltaTime_);

}

void GameCamera::ImGuiDraw()
{

	ImGui::Begin("GameCamera");
	if (upsideDown_) {
		ImGui::Text("上下反転あり  Y");
	}
	else {
		ImGui::Text("上下反転なし  Y");
	}

	if (leftRightFlip_) {
		ImGui::Text("左右反転あり  X");
	}
	else {
		ImGui::Text("左右反転なし  X");
	}

	ImGui::Text("カメラ速度 %f", rotateSpeed_);
	ImGui::Text("LBで-  RBで+");

	ImGui::End();

}

Vector3 GameCamera::OffsetCalc() const
{

	//追従対象からカメラまでのオフセット
	Vector3 offset = { offsetSide_, offsetHeight_, offsetLength_ };

	Matrix4x4 rotateMatrix;

	//カメラの角度から回転行列を計算する
	rotateMatrix = Matrix4x4::MakeRotateXYZMatrix(transform_.rotate);

	//オフセットをカメラの回転に合わせて回転させる
	offset = Matrix4x4::TransformNormal(offset, rotateMatrix);

	return offset;

}

void GameCamera::ApplyGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "GameCamera";

	moveRate_ = globalVariables->GetFloatValue(groupName, "moveRate");
	rotateRate_ = globalVariables->GetFloatValue(groupName, "rotateRate");
	offsetLength_ = globalVariables->GetFloatValue(groupName, "offsetLength");
	offsetHeight_ = globalVariables->GetFloatValue(groupName, "offsetHeight");
	offsetSide_ = globalVariables->GetFloatValue(groupName, "offsetSide");
	destinationAngleX_ = globalVariables->GetFloatValue(groupName, "destinationAngleX");
	destinationAngleXAddMax_ = globalVariables->GetFloatValue(groupName, "destinationAngleXAddMax");
	fieldDown_ = globalVariables->GetFloatValue(groupName, "fieldDown");
	fieldTop_ = globalVariables->GetFloatValue(groupName, "fieldTop");
	targetFovYMax_ = globalVariables->GetFloatValue(groupName, "targetFovYMax");
	targetFovYMin_ = globalVariables->GetFloatValue(groupName, "targetFovYMin");
	fovYChangeLine_ = globalVariables->GetFloatValue(groupName, "fovYChangeLine");

}

void GameCamera::RegistrationGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "GameCamera";
	//グループを追加
	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "moveRate", moveRate_);
	globalVariables->AddItem(groupName, "rotateRate", rotateRate_);
	globalVariables->AddItem(groupName, "offsetLength", offsetLength_);
	globalVariables->AddItem(groupName, "offsetHeight", offsetHeight_);
	globalVariables->AddItem(groupName, "offsetSide", offsetSide_);
	globalVariables->AddItem(groupName, "destinationAngleX", destinationAngleX_);
	globalVariables->AddItem(groupName, "destinationAngleXAddMax", destinationAngleXAddMax_);
	globalVariables->AddItem(groupName, "fieldDown", fieldDown_);
	globalVariables->AddItem(groupName, "fieldTop", fieldTop_);
	globalVariables->AddItem(groupName, "targetFovYMax", targetFovYMax_);
	globalVariables->AddItem(groupName, "targetFovYMin", targetFovYMin_);
	globalVariables->AddItem(groupName, "fovYChangeLine", fovYChangeLine_);

}

void GameCamera::Automatic()
{

	// プレイヤーの位置
	Vector3 playerPos = player_->GetWorldTransformAdress()->GetWorldPosition();

	// ステージ中心からプレイヤー
	Vector2 toPlayer = { playerPos.x - stageCenter_.x, playerPos.z - stageCenter_.z };
	// 正規化
	toPlayer = Vector2::Normalize(toPlayer);

	// 手前左
	if (toPlayer.x <= 0.0f && toPlayer.y <= 0.0f) {
		destinationAngleY_ = Math::LerpShortAngle(0.0f, 1.57f, fabsf(toPlayer.x));
	}
	// 奥左
	else if (toPlayer.x <= 0.0f && toPlayer.y > 0.0f) {
		destinationAngleY_ = Math::LerpShortAngle(3.14f, 1.57f, fabsf(toPlayer.x));
	}
	// 手前右
	else if (toPlayer.x > 0.0f && toPlayer.y <= 0.0f) {
		destinationAngleY_ = Math::LerpShortAngle(0.0f, -1.57f, fabsf(toPlayer.x));
	}
	// 奥右
	else {
		destinationAngleY_ = Math::LerpShortAngle(3.14f, -1.57f, fabsf(toPlayer.x));
	}
	transform_.rotate.y = Math::LerpShortAngle(transform_.rotate.y, destinationAngleY_, rotateRate_);

	//X

	// プレイヤーの高さ確認
	float destinationAngleXAdd = 0.0f;

	float addRateNumerator = playerPos.y - fieldDown_; // 分子
	float addRateDenominator = fieldTop_ - fieldDown_;// 分母

	float addRate = addRateNumerator / addRateDenominator;
	addRate = std::fmaxf(addRate, 0.0f);
	addRate = std::fminf(addRate, 1.0f);

	destinationAngleXAdd = Ease::Easing(Ease::EaseName::Lerp, 0.0f, destinationAngleXAddMax_, addRate);

	transform_.rotate.x = destinationAngleX_ + destinationAngleXAdd;

}

void GameCamera::Manual()
{

	// スティック入力で角度を変更処理

	float speed = baseRotateSpeed_ * rotateSpeed_;

	float upsideDownValue = 1.0f;
	if (upsideDown_) {
		upsideDownValue = -1.0f;
	}

	float leftRightFlipValue = 1.0f;

	if (leftRightFlip_) {
		leftRightFlipValue = -1.0f;
	}

	destinationAngleY_ += input_->GetRightAnalogstick().x * speed * leftRightFlipValue;
	manualDestinationAngleX_ += input_->GetRightAnalogstick().y * speed * upsideDownValue;

	// xに制限
	float limit = 3.14f / 4.0f;
	manualDestinationAngleX_ = std::clamp(manualDestinationAngleX_, 0.0f, limit);

	transform_.rotate.y = Math::LerpShortAngle(transform_.rotate.y, destinationAngleY_, rotateRate_);
	transform_.rotate.x = Math::LerpShortAngle(transform_.rotate.x, manualDestinationAngleX_, rotateRate_);

}

void GameCamera::Setting()
{

	// 上下反転
	if (input_->TriggerJoystick(JoystickButton::kJoystickButtonY)) {
		if (upsideDown_) {
			upsideDown_ = false;
		}
		else {
			upsideDown_ = true;
		}
	}
	// 左右反転
	if (input_->TriggerJoystick(JoystickButton::kJoystickButtonX)) {
		if (leftRightFlip_) {
			leftRightFlip_ = false;
		}
		else {
			leftRightFlip_ = true;
		}
	}

	// カメラ感度調整

	float rotateSpeedAdd = 0.0f;
	if(input_->PushJoystick(JoystickButton::kJoystickButtonRB)){
		rotateSpeedAdd += 0.01f;
	}
	if (input_->PushJoystick(JoystickButton::kJoystickButtonLB)) {
		rotateSpeedAdd -= 0.01f;
	}

	rotateSpeed_ = std::clamp(rotateSpeed_ + rotateSpeedAdd, 0.5f, 7.0f);

}
