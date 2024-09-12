#include "TutorialSystem.h"

#include "../../../Engine/Object/BaseObjectManager.h"
#include "../../Object/Character/Player/Player.h"
#include "../../Object/Obstacle/Block/BlockManager.h"
#include "../../../Engine/Math/DeltaTime.h"
#include "../../../Engine/3D/ModelDraw.h"
#include "../../../Engine/Particle/BillBoardMatrix.h"
#include "../../Camera/GameCamera.h"

void TutorialSystem::Initialize(BaseObjectManager* objectManager, Player* player, BlockManager* blockManager, GameCamera* gameCamera)
{

	objectManager_ = objectManager;

	player_ = player;

	blockManager_ = blockManager;

	gameCamera_ = gameCamera;

	tutorialFlowNumber_ = kTutorialFlowStartCheck;

	tutorialFlowUpdates_[kTutorialFlowStartCheck] = std::bind(&TutorialSystem::StartCheck, this);
	tutorialFlowUpdates_[kTutorialFlowJumpCheck] = std::bind(&TutorialSystem::JumpCheck, this);
	tutorialFlowUpdates_[kTutorialFlowUpperRowOccurrence] = std::bind(&TutorialSystem::UpperRowOccurrence, this);
	tutorialFlowUpdates_[kTutorialFlowKnockFromBelowCheck] = std::bind(&TutorialSystem::KnockFromBelowCheck, this);
	tutorialFlowUpdates_[kTutorialFlowLowerRowOccurrence] = std::bind(&TutorialSystem::LowerRowOccurrence, this);
	tutorialFlowUpdates_[kTutorialFlowFallingAttackCheck] = std::bind(&TutorialSystem::FallingAttackCheck, this);
	tutorialFlowUpdates_[kTutorialFlowEndSystem] = std::bind(&TutorialSystem::EndSystem, this);

	startCheckStruct_.center_ = { 15.0f,3.0f,-15.0f };
	startCheckStruct_.radius_ = 3.0f;

	jumpCheckStruct_.isJumpClear_ = false;
	jumpCheckStruct_.isSmallJumpClear_ = false;

	StartPosObjectInitialize();

	TutorialArrowObjectInitialize();

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

void TutorialSystem::FallingAttackCheck()
{
}

void TutorialSystem::UpperRowOccurrence()
{
}

void TutorialSystem::LowerRowOccurrence()
{
}

void TutorialSystem::StartCheck()
{
	// 範囲内に入った
	float length = Vector3::Length(player_->GetWorldTransformAdress()->GetWorldPosition() - startCheckStruct_.center_);
	if (length <= startCheckStruct_.radius_) {
		tutorialFlowNumber_ = kTutorialFlowJumpCheck;
		blockManager_->GetBlocks()->at(5)->SetIsRockMove(false);
	}

	BaseCamera camera = *gameCamera_;
	tutorialArrowObject_->Update(BillBoardMatrix::GetBillBoardMatrixY(camera));

}

void TutorialSystem::EndSystem()
{
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

	blockManager_->GetBlocks()->at(5)->Down();
	blockManager_->GetBlocks()->at(5)->SetIsRockMove(true);

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

void TutorialSystem::SpriteDraw()
{
}

void TutorialSystem::JumpCheck()
{

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
			tutorialFlowNumber_ = kTutorialFlowUpperRowOccurrence;
		}

	}

}

void TutorialSystem::KnockFromBelowCheck()
{
}
