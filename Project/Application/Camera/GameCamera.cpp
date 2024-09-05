#include "GameCamera.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../../Engine/Math/Math.h"
#include "../../../Engine/Math/Ease.h"

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
	offsetLength_ = -50.0f;

	// オフセット高さ
	offsetHeight_ = 3.0f;

	RegistrationGlobalVariables();
	ApplyGlobalVariables();

}

void GameCamera::Update(float elapsedTime)
{

	assert(player_);

#ifdef _DEMO
	ApplyGlobalVariables();
#endif // _DEBUG

	// スティック入力で角度を変更処理

	const float RotateSpeed = 0.05f;

	// カメラから見てプレイヤーが右か左か
	Vector3 playerPos = player_->GetWorldTransformAdress()->GetWorldPosition();
	Vector2 toCenter = { stageCenter_.x - transform_.translate.x, stageCenter_.z - transform_.translate.z };
	Vector2 toPlayer = { playerPos.x - transform_.translate.x, playerPos.z - transform_.translate.z };

	float cross = Vector2::Cross(toCenter, toPlayer);

	float dir = 0.0f;

	if (fabsf(cross) <= 0.2f) {
		//transform_.rotate.y = destinationAngleY_;
		//transform_.rotate.x = destinationAngleX_;
	}
	else {
		if (cross != 0.0f) {
			if (cross < 0.0f) {
				dir = -1.0f;
			}
			else {
				dir = 1.0f;
			}
		}

		destinationAngleY_ += dir * RotateSpeed;

		transform_.rotate.y = Math::LerpShortAngle(transform_.rotate.y, destinationAngleY_, rotateRate_);
		transform_.rotate.x = destinationAngleX_;
	}

	// 追従対象がいれば
	// 追従座標の補間
	interTarget_ = Ease::Easing(Ease::EaseName::Lerp, interTarget_, stageCenter_, moveRate_);

	// オフセット
	Vector3 offset = OffsetCalc();

	transform_.translate = Vector3::Add(interTarget_, offset);

	//ビュー更新
	BaseCamera::Update();

}

Vector3 GameCamera::OffsetCalc() const
{

	//追従対象からカメラまでのオフセット
	Vector3 offset = { 0.0f, offsetHeight_, offsetLength_ };

	Matrix4x4 rotateMatrix;

	//カメラの角度から回転行列を計算する
	rotateMatrix = Matrix4x4::MakeRotateXYZMatrix(transform_.rotate);

	//オフセットをカメラの回転に合わせて回転させる
	offset = Matrix4x4::TransformNormal(offset, rotateMatrix);

	return offset;

}

void GameCamera::ApplyGlobalVariables()
{
}

void GameCamera::RegistrationGlobalVariables()
{
}
