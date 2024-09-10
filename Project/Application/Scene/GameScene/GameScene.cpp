#include "GameScene.h"
#include "../../../Engine/base/WinApp.h"
#include "../../../Engine/base/TextureManager.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/base/D3DResourceLeakChecker.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../Particle/EmitterName.h"
#include "../../../Engine/Math/DeltaTime.h"
#include "../../../Engine/3D/ModelDraw.h"
#include "../../../Engine/base/WindowSprite.h"
#include "../../Object/Manager/GameSceneObjectManager.h"
#include "../../Object/ObjectFactory.h"

#include "../../Object/Character/Player/Player.h"
#include "../../Object/Obstacle/Block/Block.h"
#include "../../../Engine/Physics/Gravity.h"
#include "../../Object/Character/Player/Horn/PlayerHorn.h"
#include "../../Object/Character/Enemy/Enemy.h"
#include "../../Object/Character/Enemy/Egg.h"
GameScene::~GameScene()
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
void GameScene::Initialize() {

	IScene::Initialize();

	ModelCreate();
	TextureLoad();

	audioManager_ = std::make_unique<GameAudioManager>();
	audioManager_->StaticInitialize();
	audioManager_->Initialize();

	// オブジェクトマネージャー
	objectManager_ = std::make_unique<GameSceneObjectManager>();
	ObjectFactory::GetInstance()->Initialize(objectManager_.get());
	objectManager_->Initialize(kLevelIndexMain, levelDataManager_);

	// 平行光源
	directionalLight_ = std::make_unique<DirectionalLight>();
	directionalLight_->Initialize();

	DirectionalLightData directionalLightData;
	directionalLightData.color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData.direction = { 0.0f, -1.0f, 0.0f };
	directionalLightData.intencity = 1.0f;

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

	gameCamera_ = std::make_unique<GameCamera>();
	gameCamera_->Initialize();

	collisionManager_.reset(new CollisionManager);
	collisionManager_->Initialize();

	// 重力設定
	Gravity::SetPower(2.45f);

	// ブロックマネージャー
	blockManager_ = std::make_unique<BlockManager>();
	blockManager_->Initialize();

	// 影
	shadowModel_.reset(Model::Create("Resources/Model/shadow/", "shadow.obj", dxCommon_));
	shadowManager_ = std::make_unique<ShadowManager>();
	shadowManager_->Initialize(shadowModel_.get());

	// 平行光源システム
	directionalLightSystem_ = std::make_unique<DirectionalLightSystem>();
	directionalLightSystem_->initialize(directionalLight_.get());

	// 背景
	backGroundTextureHandle_ = TextureManager::Load("Resources/default/white2x2.png", dxCommon_);
	backGround_ = std::make_unique<BackGround>();
	backGround_->Initialize(backGroundTextureHandle_);

	// ここからオブジェクト生成

	// プレイヤー
	CreatePlayer();

	//敵(仮)
	//CreateEnemy();

	//敵マネージャー
	enemyManager_ = std::make_unique<EnemyManager>();
	enemyManager_->Initialize();
	enemyManager_->SetPlayer(player_);
	enemyManager_->SetObjectManager(objectManager_.get());
	enemyManager_->SetBlockManager(blockManager_.get());

	// ブロック
	CreateBlocks();


	// 雲
	cloudSystem_ = std::make_unique<CloudSystem>();
	cloudSystem_->Initialize(
		dxCommon_->GetDevice(),
		dxCommon_->GetCommadListLoad(),
		GraphicsPipelineState::sRootSignature[GraphicsPipelineState::kPipelineStateIndexGPUParticle].Get(),
		GraphicsPipelineState::sPipelineState[GraphicsPipelineState::kPipelineStateIndexGPUParticle].Get(),
		player_);

	// UIシステム
	UISystem_ = std::make_unique<UISystem>();
	UISystem_->Initialize(dxCommon_);

	IScene::InitilaizeCheck();

}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update() {

#ifdef _DEMO

	// デバッグ用 クリアシーンへ
	if (input_->TriggerJoystick(JoystickButton::kJoystickButtonBACK)) {
		// 行きたいシーンへ
		requestSceneNo_ = kClear;
	}

#endif // _DEMO

	enemyManager_->Update();

	objectManager_->Update();

	// カメラ
	gameCamera_->Update();

	camera_ = static_cast<BaseCamera>(*gameCamera_.get());

	// デバッグカメラ
	DebugCameraUpdate();

	// あたり判定
	collisionManager_->ListClear();

	objectManager_->CollisionListRegister(collisionManager_.get());

	collisionManager_->CheakAllCollision();

	// 影
	ShadowUpdate();

	// 平行光源
	directionalLightSystem_->Update();
	// 背景
	backGround_->Update(directionalLightSystem_->GetDirectionalLightData().color);
	// 雲
	cloudSystem_->Update(directionalLightSystem_->GetDirectionalLightData().color);

	// エネミーマネージャー後処理
	enemyManager_->PostUpdate();

	// UI
	UISystem_->Update();

	ImguiDraw();

}

/// <summary>
/// 描画処理
/// </summary>
void GameScene::Draw() {

#pragma region 背景スプライト描画

	// 背景スプライト描画前処理
	Sprite::PreDraw(dxCommon_->GetCommadList());

	backGround_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	renderTargetTexture_->ClearDepthBuffer();

#pragma endregion

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

	// 影
	shadowManager_->Draw(camera_);

	ModelDraw::PostDraw();

#pragma endregion

#pragma region 線描画

	drawLine_->Draw(dxCommon_->GetCommadList(), camera_);

#pragma endregion

	// パーティクル描画
	objectManager_->ParticleDraw(camera_);

	// 雲
	cloudSystem_->Draw(dxCommon_->GetCommadList(), camera_);

	// エネミー
	enemyManager_->ParticleDraw(camera_);

	// スプライト描画前処理
	Sprite::PreDraw(dxCommon_->GetCommadList());

	// UI
	UISystem_->Draw();

	// 前景スプライト描画後処理
	Sprite::PostDraw();

}

void GameScene::ImguiDraw(){

	ImGui::Begin("GameScene");
	ImGui::Text("Frame rate: %6.2f fps", ImGui::GetIO().Framerate);
	ImGui::End();

	debugCamera_->ImGuiDraw();

	// オブジェクトマネージャー
	objectManager_->ImGuiDraw();

}

void GameScene::DebugCameraUpdate()
{

#ifdef _DEMO
	if (input_->TriggerKey(DIK_RSHIFT)) {
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

void GameScene::ModelCreate()
{
}

void GameScene::TextureLoad()
{

	skyboxTextureHandle_ = TextureManager::Load("Resources/default/rostock_laage_airport_4k.dds", DirectXCommon::GetInstance());

}

void GameScene::LowerVolumeBGM()
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

void GameScene::CreateBlocks() {
	
	LevelData::ObjectData data;
	
	IObject* pointer = nullptr;
	
	for (size_t z = 0; z < Block::kNumOnece_; z++) {
		for (size_t x = 0; x < Block::kNumOnece_;x++) {
			data = Block::BlockCreate();
			LevelData::MeshData &block = std::get<LevelData::MeshData>(data);
			block.transform.translate.x = (float(x) - float(Block::kNumOnece_-1)*0.5f)*2.0f * Block::kSize_;
			block.transform.translate.z = (float(z) - float(Block::kNumOnece_-1)*0.5f)*2.0f * Block::kSize_;
			block.transform.translate.y = -2.0f;
			pointer = objectManager_->AddObject(data);
			
			// ブロックマネージャーに登録
			blockManager_->AddBlock(static_cast<Block*>(pointer));
		}
	}

}

void GameScene::CreatePlayer()
{

	LevelData::ObjectData data;

	IObject* pointer = nullptr;

	// プレイヤー本体
	data = Player::PlayerCreate();
	pointer =  objectManager_->AddObject(data);
	player_ = static_cast<Player*>(pointer);
	player_->SetCamera(&camera_);
	player_->SetBlockManager(blockManager_.get());
	player_->SetAudioManager(audioManager_.get());
	
	// カメラにプレイヤー設定
	gameCamera_->SetPlayer(player_);

	// 右角
	data = PlayerHorn::PlayerHornCreate("RightPlayerHorn");
	pointer = objectManager_->AddObject(data);
	PlayerHorn* playerHorn = static_cast<PlayerHorn*>(pointer);
	playerHorn->SetParent(player_, "RightHorn");

	// 左角
	data = PlayerHorn::PlayerHornCreate("LeftPlayerHorn");
	pointer = objectManager_->AddObject(data);
	playerHorn = static_cast<PlayerHorn*>(pointer);
	playerHorn->SetParent(player_, "LeftHorn");

}

void GameScene::ShadowUpdate()
{

	// 前処理
	shadowManager_->PreUpdate();

	// 登録
	
	// 影を発生させるオブジェクト
	// プレイヤー
	shadowManager_->CastsShadowObjListRegister(player_);

	// エネミー
	std::list<BaseEnemy*>::iterator itrEnemy = enemyManager_->GetEnemys()->begin();
	for (; itrEnemy != enemyManager_->GetEnemys()->end(); ++itrEnemy) {
		
		BaseEnemy* enemy = *itrEnemy;
		shadowManager_->CastsShadowObjListRegister(enemy);

	}

	// 卵
	std::list<Egg*>::iterator itrEgg = enemyManager_->GetEggs()->begin();
	for (; itrEgg != enemyManager_->GetEggs()->end(); ++itrEgg) {

		Egg* egg = *itrEgg;
		shadowManager_->CastsShadowObjListRegister(egg);

	}

	// 影が現れるオブジェクト
	// ブロック
	for (uint32_t i = 0; i < blockManager_->GetBlockNum(); ++i) {
		shadowManager_->ShadowAppearsObjListRegister(blockManager_->GetBlocks()->at(i));
	}

	// 更新
	shadowManager_->Update();

}

void GameScene::CreateEnemy()
{

	LevelData::ObjectData data;

	IObject* pointer = nullptr;

	data = Egg::EggCreate();
	LevelData::MeshData& enemy = std::get<LevelData::MeshData>(data);
	enemy.transform.translate.x = -8.0f;
	enemy.transform.translate.y = 16.0f;
	pointer = objectManager_->AddObject(data);
	static_cast<Egg*>(pointer)->SetPlayer(player_);
	static_cast<Egg*>(pointer)->SetBlockManager(blockManager_.get());
	static_cast<Egg*>(pointer)->SetObjectManager(objectManager_.get());
	
}