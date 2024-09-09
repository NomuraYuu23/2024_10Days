#pragma once
#include <array>

#include "../../../Engine/Scene/IScene/IScene.h"
#include "../../AudioManager/GameAudioManager.h"
#include "../../Skydome/Skydome.h"

#include "../../Object/Sample/SampleObject.h" // サンプルオブジェクト

#include "../../../Engine/Light/DirectionalLight/DirectionalLight.h" // 平行光源
#include "../../../Engine/Light/PointLight/PointLightManager.h" // 点光源
#include "../../../Engine/Light/SpotLight/SpotLightManager.h" // スポット

#include "../../../Engine/Collision2D/Collision2DManager.h"
#include "../../../Engine/Collision2D/Collision2DDebugDraw.h"

#include "../../Camera/FollowCamera.h"
#include "../../Camera/GameCamera.h"
#include "../../Object/Obstacle/Block/BlockManager.h"
#include "../../System/Shadow/ShadowManager.h"
#include "../../System/Light/DirectionalLightSystem.h"
#include "../../System/BackGround/BackGround.h"
#include "../../System/CloudSystem/CloudSystem.h"
#include "../../UI/UISystem.h"

// デバッグ
#include "../../GPUParticle/EggBreakParticle/EggBreakParticle.h"

class GameScene : public IScene
{

public:

	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// imgui描画処理
	/// </summary>
	void ImguiDraw();

public: //メンバ関数

	/// <summary>
	/// デバッグカメラ更新
	/// </summary>
	void DebugCameraUpdate();

private: // メンバ関数

	/// <summary>
	/// モデルクリエイト
	/// </summary>
	void ModelCreate() override;

	/// <summary>
	/// テクスチャロード
	/// </summary>
	void TextureLoad() override;

private:
	  
	/// <summary>
	/// 音量
	/// </summary>
	void LowerVolumeBGM();

	/// <summary>
	/// ブロック初期配置
	/// </summary>
	void CreateBlocks();

	/// <summary>
	/// プレイヤー作成
	/// </summary>
	void CreatePlayer();

	/// <summary>
	/// 敵作成(仮)
	/// </summary>
	void CreateEnemy();

	/// <summary>
	/// 影更新
	/// </summary>
	void ShadowUpdate();

private:

	// オーディオマネージャー
	std::unique_ptr<GameAudioManager> audioManager_;
	bool isDecreasingVolume = true;

	// スカイボックス
	uint32_t skyboxTextureHandle_ = 0;

	// 衝突マネージャー
	std::unique_ptr<CollisionManager> collisionManager_;

	// ゲームカメラ
	std::unique_ptr<GameCamera> gameCamera_;

	// ブロックマネージャー
	std::unique_ptr<BlockManager> blockManager_;

	// 影マネージャー
	std::unique_ptr<ShadowManager> shadowManager_;
	std::unique_ptr<Model> shadowModel_;

	// プレイヤー
	Player* player_;

	// 平行光源システム
	std::unique_ptr<DirectionalLightSystem> directionalLightSystem_;

	// 背景
	std::unique_ptr<BackGround> backGround_;
	uint32_t backGroundTextureHandle_ = 0;

	// 雲
	std::unique_ptr<CloudSystem> cloudSystem_;

	// UIシステム
	std::unique_ptr<UISystem> UISystem_;

};
