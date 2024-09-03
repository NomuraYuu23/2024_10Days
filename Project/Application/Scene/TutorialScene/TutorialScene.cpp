#include "TutorialScene.h"
#include "../../../Engine/base/WinApp.h"
#include "../../../Engine/base/TextureManager.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/base/D3DResourceLeakChecker.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../Particle/EmitterName.h"
#include "../../../Engine/Math/DeltaTime.h"
#include "../../../Engine/3D/ModelDraw.h"
#include "../../../Engine/base/OutputLog.h"
#include "../../Object/Manager/TutorialSceneObjectManager.h"
#include "../../Object/ObjectFactory.h"

TutorialScene::~TutorialScene()
{

	if (stopAudio_) {
		for (uint32_t i = 0; i < audioManager_->kMaxPlayingSoundData; ++i) {
			audioManager_->StopWave(i);
		}
	}

}

/// <summary>
/// 初期化
/// </summary>
void TutorialScene::Initialize() {

	IScene::Initialize();

	ModelCreate();
	TextureLoad();

	audioManager_ = std::make_unique<GameAudioManager>();
	audioManager_->StaticInitialize();
	audioManager_->Initialize();

	// オブジェクトマネージャー
	objectManager_ = std::make_unique<TutorialSceneObjectManager>();
	ObjectFactory::GetInstance()->Initialize(objectManager_.get(), nullptr);
	objectManager_->Initialize(kLevelIndexTitle, levelDataManager_);

	// 平行光源
	directionalLight_ = std::make_unique<DirectionalLight>();
	directionalLight_->Initialize();

	DirectionalLightData directionalLightData;
	directionalLightData.color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData.direction = { 0.0f, -1.0f, 0.0f };
	directionalLightData.intencity = 0.0f;

	directionalLight_->Update(directionalLightData);

	// 点光源
	pointLightManager_ = std::make_unique<PointLightManager>();
	pointLightManager_->Initialize();
	for (size_t i = 0; i < pointLightDatas_.size(); ++i) {
		pointLightDatas_[i].color = { 1.0f,1.0f,1.0f,1.0f };
		pointLightDatas_[i].position = { 0.0f, -1.0f, 0.0f };
		pointLightDatas_[i].intencity = 1.0f;
		pointLightDatas_[i].radius = 10.0f;
		pointLightDatas_[i].decay = 10.0f;
		pointLightDatas_[i].used = false;
	}

	pointLightDatas_[0].color = { 0.93f, 0.47f, 0.0f, 1.0f };
	pointLightDatas_[0].position = { 0.0f, 0.0f, 0.0f };
	pointLightDatas_[0].intencity = 1.0f;
	pointLightDatas_[0].radius = 50.0f;
	pointLightDatas_[0].decay = 10.0f;
	pointLightDatas_[0].used = true;

	pointLightManager_->Update(pointLightDatas_);

	spotLightManager_ = std::make_unique<SpotLightManager>();
	spotLightManager_->Initialize();
	for (size_t i = 0; i < spotLightDatas_.size(); ++i) {
		spotLightDatas_[i].color = { 1.0f,1.0f,1.0f,1.0f };
		spotLightDatas_[i].position = { 0.0f, -1.0f, 0.0f };
		spotLightDatas_[i].intencity = 1.0f;
		spotLightDatas_[i].direction = { 0.0f, -1.0f, 0.0f }; // ライトの方向
		spotLightDatas_[i].distance = 10.0f; // ライトの届く距離
		spotLightDatas_[i].decay = 2.0f; // 減衰率
		spotLightDatas_[i].cosAngle = 2.0f; // スポットライトの余弦
		spotLightDatas_[i].cosFalloffStart = 1.0f; // フォールオフ開始位置
		spotLightDatas_[i].used = false; // 使用している
	}

	EulerTransform cameraTransform = {
		1.0f,1.0f,1.0f,
		0.03f, -0.45f, 0.0f,
		6.0f, 1.7f, -9.5f };

	camera_.SetTransform(cameraTransform);
	camera_.Update();

	IScene::InitilaizeCheck();

}

/// <summary>
/// 更新処理
/// </summary>
void TutorialScene::Update() {

	if (input_->TriggerJoystick(JoystickButton::kJoystickButtonA)) {
		// 行きたいシーンへ
		requestSceneNo_ = kTutorial;
	}

	objectManager_->Update();

	// デバッグカメラ
	DebugCameraUpdate();

	ImguiDraw();

}

/// <summary>
/// 描画処理
/// </summary>
void TutorialScene::Draw() {

#pragma region モデル描画

	ModelDraw::PreDrawDesc preDrawDesc;
	preDrawDesc.commandList = dxCommon_->GetCommadList();
	preDrawDesc.directionalLight = directionalLight_.get();
	preDrawDesc.fogManager = FogManager::GetInstance();
	preDrawDesc.pointLightManager = pointLightManager_.get();
	preDrawDesc.spotLightManager = spotLightManager_.get();
	preDrawDesc.environmentTextureHandle = skyboxTextureHandle_;
	ModelDraw::PreDraw(preDrawDesc);

	//3Dオブジェクトはここ

	objectManager_->Draw(camera_, drawLine_);

	ModelDraw::PostDraw();

#pragma endregion

	// パーティクル描画
	objectManager_->ParticleDraw(camera_);

	// スプライト描画前処理
	Sprite::PreDraw(dxCommon_->GetCommadList());

	// 前景スプライト描画後処理
	Sprite::PostDraw();

}

void TutorialScene::ImguiDraw() {

	ImGui::Begin("Framerate");
	ImGui::Text("Frame rate: %6.2f fps", ImGui::GetIO().Framerate);
	ImGui::End();

	debugCamera_->ImGuiDraw();

	// オブジェクトマネージャー
	objectManager_->ImGuiDraw();

}

void TutorialScene::DebugCameraUpdate()
{

#ifdef _DEMO
	if (input_->TriggerKey(DIK_RETURN)) {
		if (isDebugCameraActive_) {
			isDebugCameraActive_ = false;
		}
		else {
			isDebugCameraActive_ = true;
		}
	}

	// カメラの処理
	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		debugCamera_->Update();
		// デバッグカメラのビュー行列をコピー
		camera_ = static_cast<BaseCamera>(*debugCamera_.get());
		// ビュー行列の転送
		camera_.Update();
	}
#endif

}

void TutorialScene::ModelCreate()
{

}

void TutorialScene::TextureLoad()
{

	skyboxTextureHandle_ = TextureManager::Load("Resources/default/rostock_laage_airport_4k.dds", DirectXCommon::GetInstance());

}

void TutorialScene::LowerVolumeBGM()
{

	const uint32_t startHandleIndex = 3;

	//for (uint32_t i = 0; i < audioManager_->kMaxPlayingSoundData; ++i) {
	//	if (audioManager_->GetPlayingSoundDatas()[i].handle_ == kGameAudioNameIndexBGM + startHandleIndex) {
	//		float decreasingVolume = 1.0f / 60.0f;
	//		float volume = audioManager_->GetPlayingSoundDatas()[i].volume_ - decreasingVolume;
	//		if (volume < 0.0f) {
	//			volume = 0.0f;
	//			audioManager_->StopWave(i);
	//			isDecreasingVolume = false;
	//		}
	//		else {
	//			audioManager_->SetPlayingSoundDataVolume(i, volume);
	//			audioManager_->SetVolume(i, audioManager_->GetPlayingSoundDatas()[i].volume_);
	//		}
	//	}
	//}

}
