#include "GameCamera.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../../Engine/Math/Math.h"
#include "../../../Engine/Math/Ease.h"

void GameCamera::Initialize()
{

	BaseCamera::Initialize();

	// プレイヤー
	player_ = nullptr;

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

	// 方向
	direction_ = { 0.0f,0.0f,1.0f };

	// 回転行列
	rotateMatrix_ = Matrix4x4::MakeIdentity4x4();

	RegistrationGlobalVariables();
	ApplyGlobalVariables();

}

void GameCamera::Update(float elapsedTime)
{

	assert(player_);

#ifdef _DEMO
	ApplyGlobalVariables();
#endif // _DEMO

	// プレイヤー
	Vector3 playerPos = player_->GetWorldTransformAdress()->GetWorldPosition();

	// 位置

	//ステージからプレイヤーの位置で方向取得
	playerPos.y = stageCenter_.y;
	Vector3	playerFromStage = Vector3::Normalize(playerPos - stageCenter_);

	//方向とカメラオフセットで目指すべきカメラ位置取得
	Vector3 targetPosition = playerFromStage * offsetLength_;
	targetPosition.y = offsetHeight_;

	//カメラの現在位置と目指すべきカメラ位置で補間
	transform_.translate = Ease::Easing(Ease::EaseName::Lerp, transform_.translate, targetPosition, moveRate_);

	// 回転

	//カメラからステージで目指す方向取得

	Vector3 pos = transform_.translate;
	pos.y = stageCenter_.y;
	Vector3 stagFromCamera = Vector3::Normalize(stageCenter_ - pos);

	////現在の方向から目指す方向で補間
	direction_.x = Ease::Easing(Ease::EaseName::Lerp, direction_.x, stagFromCamera.x, rotateRate_);
	direction_.y = stagFromCamera.y;
	direction_.z = Ease::Easing(Ease::EaseName::Lerp, direction_.z, stagFromCamera.z, rotateRate_);
	//direction_ = Ease::Easing(Ease::EaseName::Lerp, direction_, stagFromCamera, rotateRate_);
	direction_ = stagFromCamera;

	// ズーム
	Zoom(elapsedTime);

	// シェイク
	if (isShake_) {
		ShakeUpdate(elapsedTime);
	}

	// マッピング
	Matrix4x4 scaleMatrix = Matrix4x4::MakeScaleMatrix(transform_.scale);
	rotateMatrix_ = Matrix4x4::DirectionToDirection(Vector3{0.0f,0.0f,1.0f}, direction_);
	Matrix4x4 translateMatrix = Matrix4x4::MakeTranslateMatrix(transform_.translate + shakeAddPosition_);
	transformMatrix_ = Matrix4x4::Multiply(scaleMatrix, Matrix4x4::Multiply(rotateMatrix_, translateMatrix));;

	viewMatrix_ = Matrix4x4::Inverse(transformMatrix_);
	projectionMatrix_ = Matrix4x4::MakePerspectiveFovMatrix(fovY_, aspectRatio_, nearClip_, farClip_);

	viewProjectionMatrix_->matrix = Matrix4x4::Multiply(viewMatrix_, projectionMatrix_);

	worldPositionMap_->worldPosition = { transformMatrix_.m[3][0],transformMatrix_.m[3][1], transformMatrix_.m[3][2] };

}

void GameCamera::ApplyGlobalVariables()
{
}

void GameCamera::RegistrationGlobalVariables()
{
}
