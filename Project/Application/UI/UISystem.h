#pragma once
#include "../../Engine/Input/Input.h"
#include "../Object/Character/Player/Player.h"
#include "UI.h"

enum UITextureHandleIndex {
	kUITextureHandleIndexStickBG, // スティック背景
	kUITextureHandleIndexLeftStick, // 左スティック
	kUITextureHandleIndexLeftStickStr, // 左スティック文字
	kUITextureHandleIndexButtonA, // Aボタン
	kUITextureHandleIndexButtonAStr, // Aボタン文字
	kUITextureHandleIndexHPHead,// HP頭
	kUITextureHandleIndexHPHorn,// HP角
	kUITextureHandleIndexRightStick, // 右スティック
	kUITextureHandleIndexRightStickStr, // 右スティック文字
	kUITextureHandleIndexHPStr, // HP文字
	kUITextureHandleIndexHP0, // 0
	kUITextureHandleIndexHP1, // 1
	kUITextureHandleIndexHP2, // 2
	kUITextureHandleIndexHP3, // 3

	kUITextureHandleIndexOfCount
};

enum UIIndex {
	kUIIndexLeftStickBG, // 左スティック背景
	kUIIndexLeftStick, // 左スティック
	kUIIndexLeftStickStr, // 左スティック文字
	kUIIndexButtonA, // Aボタン
	kUIIndexButtonAStr, // Aボタン文字
	kUIIndexRightStickBG, // 右スティック背景
	kUIIndexRightStick, // 右スティック
	kUIIndexRightStickStr, // 右スティック文字
	kUIIndexHP1,// HP1
	kUIIndexHP0,// HP0
	kUIIndexHP2,// HP2
	kUIIndexHPStr, // HP文字
	kUIIndexHPNum, // HP数字
	kUIIndexOfCount
};

class UISystem
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="dxCommon"></param>
	/// <param name="player"></param>
	void Initialize(DirectXCommon* dxCommon, Player* player);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// タイトルの追加位置設定
	/// </summary>
	/// <param name="titlePosAddX"></param>
	void SetTitlePosAddX(float titlePosAddX) { titlePosAddX_ = titlePosAddX; }

	/// <summary>
	///	チュートリアルの追加位置設定
	/// </summary>
	/// <param name="tutorialPosAddX"></param>
	void SetTutorialPosAddX(float tutorialPosAddX) { tutorialPosAddX_ = tutorialPosAddX; }

private:

	// プレイヤー
	Player* player_;

	// テクスチャハンドル
	std::array<uint32_t, UITextureHandleIndex::kUITextureHandleIndexOfCount> textureHandles_;

	// UI
	std::array<std::unique_ptr<UI>, UIIndex::kUIIndexOfCount> UIs_;

	// タイトルの追加位置
	float titlePosAddX_;

	// チュートリアルの追加位置
	float tutorialPosAddX_;

private: 

	// テクスチャのパス
	const std::array<std::string, UITextureHandleIndex::kUITextureHandleIndexOfCount> textureNames_ = {
		"Resources/UI/joystick_BG.png",
		"Resources/UI/joystick_L.png",
		"Resources/UI/moveUI.png",
		"Resources/UI/buttonA.png",
		"Resources/UI/jumpUI.png",
		"Resources/UI/HP_head.png",
		"Resources/UI/HP_Horn.png",
		"Resources/UI/joystick_R.png",
		"Resources/UI/cameraUI.png",
		"Resources/UI/HP_UI.png",
		"Resources/Sprite/nums/0.png",
		"Resources/Sprite/nums/1.png",
		"Resources/Sprite/nums/2.png",
		"Resources/Sprite/nums/3.png",
	};

};

