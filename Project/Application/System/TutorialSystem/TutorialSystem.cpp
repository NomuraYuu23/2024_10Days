#include "TutorialSystem.h"

#include "../../../Engine/Object/BaseObjectManager.h"
#include "../../Object/Character/Player/Player.h"
#include "../../Object/Obstacle/Block/BlockManager.h"
#include "../../../Engine/Math/DeltaTime.h"
#include "../../../Engine/3D/ModelDraw.h"
#include "../../../Engine/Particle/BillBoardMatrix.h"
#include "../../Camera/GameCamera.h"
#include "../../../Engine/base/TextureManager.h"
#include "../../../Engine/Math/Ease.h"
#include "../../../Engine/Math/DeltaTime.h"
#include "../../Object/Character/Enemy/EnemyManager.h"

#include "../../../externals/imgui/imgui.h"

void TutorialSystem::Initialize(
	BaseObjectManager* objectManager, 
	Player* player, 
	BlockManager* blockManager, 
	GameCamera* gameCamera,
	EnemyManager* enemyManager)
{

	objectManager_ = objectManager;

	player_ = player;

	blockManager_ = blockManager;

	gameCamera_ = gameCamera;

	enemyManager_ = enemyManager;

	tutorialFlowNumber_ = kTutorialFlowStartCheck;

	tutorialFlowUpdates_[kTutorialFlowStartCheck] = std::bind(&TutorialSystem::StartCheck, this);
	tutorialFlowUpdates_[kTutorialFlowJumpCheck] = std::bind(&TutorialSystem::JumpCheck, this);
	tutorialFlowUpdates_[kTutorialFlowUpperRowOccurrence] = std::bind(&TutorialSystem::UpperRowOccurrence, this);
	tutorialFlowUpdates_[kTutorialFlowKnockFromBelowCheck] = std::bind(&TutorialSystem::KnockFromBelowCheck, this);
	tutorialFlowUpdates_[kTutorialFlowLowerRowOccurrence] = std::bind(&TutorialSystem::LowerRowOccurrence, this);
	tutorialFlowUpdates_[kTutorialFlowFallingAttackCheck] = std::bind(&TutorialSystem::FallingAttackCheck, this);
	tutorialFlowUpdates_[kTutorialFlowEndSystem] = std::bind(&TutorialSystem::EndSystem, this);

	startCheckStruct_.center_ = { 21.0f,3.0f,-21.0f };
	startCheckStruct_.radius_ = 3.0f;

	jumpCheckStruct_.isJumpClear_ = false;
	jumpCheckStruct_.isSmallJumpClear_ = false;

	isEndFlow_ = false;

	StartPosObjectInitialize();

	TutorialArrowObjectInitialize();

	SpriteInitialize();

}

void TutorialSystem::Update()
{

	// 現在の更新処理
	tutorialFlowUpdates_[tutorialFlowNumber_]();

}

void TutorialSystem::Draw(BaseCamera& camera)
{

	if (tutorialFlowNumber_ == kTutorialFlowStartCheck) {
		tutorialArrowObject_->Draw(camera);
		startPosObject_->Draw(camera);
	}

}

void TutorialSystem::StartPosObjectInitialize()
{

	startPosObject_ = std::make_unique<MeshObject>();

	LevelData::MeshData data;
	// 名前
	data.name = "TutorialObject";
	// トランスフォーム
	data.transform = {
		startCheckStruct_.radius_,startCheckStruct_.radius_,startCheckStruct_.radius_,
		0.0f,0.0f,0.0f,
		startCheckStruct_.center_
	};

	// ファイルの名前
	data.flieName = "Cube.obj";
	// ディレクトリパス
	data.directoryPath = "Resources/default/";
	// クラスの名前
	data.className = "";
	// 親の名前
	data.parentName = "";

	// コライダー(一時的なもの、親部分はヌルにしとく)
	OBB obb;
	obb.Initialize({ 0.0f,0.0f,0.0f }, Matrix4x4::MakeIdentity4x4(), { 1.0f,1.0f,1.0f }, static_cast<Null*>(nullptr));
	data.collider = obb;

	startPosObject_->Initialize(&data);
	Vector4 color = { 0.8f, 0.0f, 0.0f, 0.5f };
	startPosObject_->SetMaterialColor(color);

	blockManager_->GetBlocks()->at(blockNumStartCheck_)->Down();
	blockManager_->GetBlocks()->at(blockNumStartCheck_)->SetIsRockMove(true);

}

void TutorialSystem::TutorialArrowObjectInitialize()
{

	tutorialArrowObject_ = std::make_unique<TutorialArrowObject>();

	LevelData::MeshData data;
	// 名前
	data.name = "TutorialArrowObject";
	// トランスフォーム
	data.transform = {
		startCheckStruct_.radius_,startCheckStruct_.radius_,startCheckStruct_.radius_,
		0.0f,0.0f,0.0f,
		startCheckStruct_.center_.x, startCheckStruct_.center_.y + startCheckStruct_.radius_ * 2, startCheckStruct_.center_.z
	};

	// ファイルの名前
	data.flieName = "tutorial.obj";
	// ディレクトリパス
	data.directoryPath = "Resources/Model/Tutorial/";
	// クラスの名前
	data.className = "";
	// 親の名前
	data.parentName = "";

	// コライダー(一時的なもの、親部分はヌルにしとく)
	OBB obb;
	obb.Initialize({ 0.0f,0.0f,0.0f }, Matrix4x4::MakeIdentity4x4(), { 1.0f,1.0f,1.0f }, static_cast<Null*>(nullptr));
	data.collider = obb;

	tutorialArrowObject_->Initialize(&data);

}

void TutorialSystem::SpriteInitialize()
{

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Vector2 positon = { notDrawPosX_, posY_ };
	Vector4 color = { 1.0f,1.0f,1.0f,1.0f };

	// ジャンプスプライト
	uint32_t textureHandle = TextureManager::Load("Resources/Sprite/tutorial/tutorial_Jump.png", dxCommon);
	jumpSprite1_.reset(Sprite::Create(textureHandle, positon, color));
	jumpSprite1_->SetSize(jumpSize_);
	jumpSprite1_->SetTextureSize(jumpTextureSize_);
	// ジャンプスプライト
	jumpSprite2_.reset(Sprite::Create(textureHandle, positon, color));
	jumpSprite2_->SetSize(jumpSize_);
	jumpSprite2_->SetTextureLeftTop(Vector2{ 0.0f, jump2TopY_ });
	jumpSprite2_->SetTextureSize(jumpTextureSize_);
	// 攻撃１スプライト
	textureHandle = TextureManager::Load("Resources/Sprite/tutorial/tutorial_attack01.png", dxCommon);
	attack1Sprite_.reset(Sprite::Create(textureHandle, positon, color));
	attack1Sprite_->SetSize(attackSize_);
	// 攻撃２スプライト
	textureHandle = TextureManager::Load("Resources/Sprite/tutorial/tutorial_attack02.png", dxCommon);
	attack2Sprite_.reset(Sprite::Create(textureHandle, positon, color));
	attack2Sprite_->SetSize(attackSize_);

}

void TutorialSystem::SpriteDraw()
{

	if (tutorialFlowNumber_ == kTutorialFlowJumpCheck) {
		jumpSprite1_->Draw();
		jumpSprite2_->Draw();
	}
	else if(tutorialFlowNumber_ == kTutorialFlowKnockFromBelowCheck) {
		attack1Sprite_->Draw();
	}
	else if (tutorialFlowNumber_ == kTutorialFlowFallingAttackCheck) {
		attack2Sprite_->Draw();
	}

}

void TutorialSystem::JumpCheck()
{

	// 確認
	if (player_->GetCurrentStateNo() == kPlayerStateJump) {

		double animTimer = player_->GetAnimationAdress()->GetAnimationDatas()->at(kPlayerMotionJump).timer;
		if (animTimer == static_cast<double>(kDeltaTime_) * player_->GetJumpCheckpointFrame()) {
			float playerVelocityY = player_->GetVelocity().y;

			if (playerVelocityY <= player_->GetSmallJumpInitialSpeed()) {
				jumpCheckStruct_.isSmallJumpClear_ = true;
			}
			else{
				jumpCheckStruct_.isJumpClear_ = true;
			}

		}

		if (jumpCheckStruct_.isSmallJumpClear_ && jumpCheckStruct_.isJumpClear_) {
			isEndFlow_ = true;
		}

	}

	// タイム
	if (isEndFlow_) {
		elapsedTime_ -= kDeltaTime_;
		if (elapsedTime_ <= 0.0f) {
			elapsedTime_ = 0.0f;
			isEndFlow_ = false;
			tutorialFlowNumber_ = kTutorialFlowUpperRowOccurrence;
			blockManager_->GetBlocks()->at(blockNumKnockFromBelowCheck_)->Up();
			blockManager_->GetBlocks()->at(blockNumKnockFromBelowCheck_)->SetIsRockMove(true);
		}
	}
	else{
		elapsedTime_ += kDeltaTime_;
		if (elapsedTime_ >= timeMax_) {
			elapsedTime_ = timeMax_;
		}
	}

	// 位置変更
	Vector2 position = { Ease::Easing(Ease::EaseName::EaseInOutCubic, notDrawPosX_, drawPosX_, elapsedTime_ / timeMax_), posY_ };
	position.y -= jump2TopY_ / 4.0f;
	jumpSprite1_->SetPosition(position);
	position.y += jump2TopY_ / 2.0f;
	jumpSprite2_->SetPosition(position);

}

void TutorialSystem::KnockFromBelowCheck()
{

	// 確認
	if (blockManager_->GetBlocks()->at(blockNumKnockFromBelowCheck_)->GetIsAttack()) {
		isEndFlow_ = true;
		blockManager_->GetBlocks()->at(blockNumKnockFromBelowCheck_)->SetIsRockMove(false);
	}

	// タイム
	if (isEndFlow_) {
		elapsedTime_ -= kDeltaTime_;
		if (elapsedTime_ <= 0.0f) {
			elapsedTime_ = 0.0f;
			isEndFlow_ = false;
			tutorialFlowNumber_ = kTutorialFlowLowerRowOccurrence;
			blockManager_->GetBlocks()->at(blockNumFallingAttackCheck_)->Down();
			blockManager_->GetBlocks()->at(blockNumFallingAttackCheck_)->SetIsRockMove(true);
		}
	}
	else {
		elapsedTime_ += kDeltaTime_;
		if (elapsedTime_ >= timeMax_) {
			elapsedTime_ = timeMax_;
		}
	}

	// 位置変更
	Vector2 position = { Ease::Easing(Ease::EaseName::EaseInOutCubic, notDrawPosX_, drawPosX_, elapsedTime_ / timeMax_), posY_ };
	attack1Sprite_->SetPosition(position);

}

void TutorialSystem::FallingAttackCheck()
{

	// 確認
	if (blockManager_->GetBlocks()->at(blockNumFallingAttackCheck_)->GetIsAttack()) {
		isEndFlow_ = true;
	}

	// タイム
	if (isEndFlow_) {
		elapsedTime_ -= kDeltaTime_;
		if (elapsedTime_ <= 0.0f) {
			elapsedTime_ = 0.0f;
			isEndFlow_ = false;
			tutorialFlowNumber_ = kTutorialFlowEndSystem;
		}
	}
	else {
		elapsedTime_ += kDeltaTime_;
		if (elapsedTime_ >= timeMax_) {
			elapsedTime_ = timeMax_;
		}
	}

	// 位置変更
	Vector2 position = { Ease::Easing(Ease::EaseName::EaseInOutCubic, notDrawPosX_, drawPosX_, elapsedTime_ / timeMax_), posY_ };
	attack2Sprite_->SetPosition(position);

}

void TutorialSystem::UpperRowOccurrence()
{

	// タイム
	elapsedTime_ += kDeltaTime_;
	if (elapsedTime_ >= timeMax_) {
		elapsedTime_ = 0.0f;
		tutorialFlowNumber_ = kTutorialFlowKnockFromBelowCheck;
		isEndFlow_ = false;
		enemyManager_->AddEgg(blockManager_->GetBlocks()->at(blockNumKnockFromBelowCheck_)->GetWorldTransformAdress()->GetWorldPosition());
	}

}

void TutorialSystem::LowerRowOccurrence()
{

	// タイム
	elapsedTime_ += kDeltaTime_;
	if (elapsedTime_ >= timeMax_) {
		elapsedTime_ = 0.0f;
		tutorialFlowNumber_ = kTutorialFlowFallingAttackCheck;
		isEndFlow_ = false;
		enemyManager_->AddEgg(blockManager_->GetBlocks()->at(blockNumFallingAttackCheck_)->GetWorldTransformAdress()->GetWorldPosition());
	}

}

void TutorialSystem::StartCheck()
{
	// 範囲内に入った
	float length = Vector3::Length(player_->GetWorldTransformAdress()->GetWorldPosition() - startCheckStruct_.center_);
	if (length <= startCheckStruct_.radius_) {
		tutorialFlowNumber_ = kTutorialFlowJumpCheck;
		blockManager_->GetBlocks()->at(blockNumStartCheck_)->SetIsRockMove(false);
	}

	BaseCamera camera = *gameCamera_;
	tutorialArrowObject_->Update(BillBoardMatrix::GetBillBoardMatrixY(camera));

}

void TutorialSystem::EndSystem()
{
}
