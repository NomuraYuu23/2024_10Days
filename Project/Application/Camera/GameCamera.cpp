#include "GameCamera.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../../Engine/Math/Math.h"
#include "../../../Engine/Math/Ease.h"
#include "../../Engine/Math/DeltaTime.h"

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

	RegistrationGlobalVariables();
	ApplyGlobalVariables();

}

void GameCamera::Update(float elapsedTime)
{

	assert(player_);

#ifdef _DEMO
	ApplyGlobalVariables();
#endif // _DEBUG

	// プレイヤーの位置
	Vector3 playerPos = player_->GetWorldTransformAdress()->GetWorldPosition();
	
	// ステージ中心からプレイヤー
	Vector2 toPlayer = { playerPos.x - stageCenter_.x, playerPos.z - stageCenter_.z };
	// 正規化
	toPlayer = Vector2::Normalize(toPlayer);

	// 手前左
	if (toPlayer.x <= 0.0f && toPlayer.y <= 0.0f) {
		destinationAngleY_ = Math::LerpShortAngle(0.0f, 1.57f, fabsf(toPlayer.x));
		transform_.rotate.y = Math::LerpShortAngle(transform_.rotate.y, destinationAngleY_, rotateRate_);
	}
	// 奥左
	else if (toPlayer.x <= 0.0f && toPlayer.y > 0.0f) {
		destinationAngleY_ = Math::LerpShortAngle(3.14f, 1.57f, fabsf(toPlayer.x));
		transform_.rotate.y = Math::LerpShortAngle(transform_.rotate.y, destinationAngleY_, rotateRate_);
	}
	// 手前右
	else if (toPlayer.x > 0.0f && toPlayer.y <= 0.0f) {
		destinationAngleY_ = Math::LerpShortAngle(0.0f, -1.57f, fabsf(toPlayer.x));
		transform_.rotate.y = Math::LerpShortAngle(transform_.rotate.y, destinationAngleY_, rotateRate_);
	}
	// 奥右
	else {
		destinationAngleY_ = Math::LerpShortAngle(3.14f, -1.57f, fabsf(toPlayer.x));
		transform_.rotate.y = Math::LerpShortAngle(transform_.rotate.y, destinationAngleY_, rotateRate_);
	}

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
