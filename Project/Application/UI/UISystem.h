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
	kUITextureHandleIndexTimeStr, // 経過時間文字
	kUITextureHandleIndexCameraReset, // カメラリセット

	kUITextureHandleIndex0, // 0
	kUITextureHandleIndex1, // 1
	kUITextureHandleIndex2, // 2
	kUITextureHandleIndex3, // 3
	kUITextureHandleIndex4, // 4
	kUITextureHandleIndex5, // 5
	kUITextureHandleIndex6, // 6
	kUITextureHandleIndex7, // 7
	kUITextureHandleIndex8, // 8
	kUITextureHandleIndex9, // 9

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
	//kUIIndexTimeStr, // 経過時間文字
	//kUIIndexTimeTenNum, // 経過時間文字10の位
	//kUIIndexTimeOneNum, // 経過時間文字1の位
	kUIIndexCameraReset, // カメラリセット
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
	/// <param name="dayCount"></param>
	void Initialize(DirectXCommon* dxCommon, Player* player, uint32_t* dayCount);

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
	
	void SetLeftTopMove(bool leftTopMove) { leftTopMove_ = leftTopMove; }

private:

	// プレイヤー
	Player* player_;

	// テクスチャハンドル
	std::array<uint32_t, UITextureHandleIndex::kUITextureHandleIndexOfCount> textureHandles_;

	// UI
	std::array<std::unique_ptr<UI>, UIIndex::kUIIndexOfCount> UIs_;

	// タイトルの追加位置
	float titlePosAddX_;

	// 経過日数
	uint32_t* dayCount_;

	// 左上の追加位置
	float leftTopPosAddX_;

	bool leftTopMove_ = false;

	float moveTimer_ = 0.0f;

	float moveTimerMax_ = 1.5f;

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
		"Resources/UI/DayCount.png",
		"Resources/UI/camera_Reset.png",

		"Resources/Sprite/nums/0.png",
		"Resources/Sprite/nums/1.png",
		"Resources/Sprite/nums/2.png",
		"Resources/Sprite/nums/3.png",
		"Resources/Sprite/nums/4.png",
		"Resources/Sprite/nums/5.png",
		"Resources/Sprite/nums/6.png",
		"Resources/Sprite/nums/7.png",
		"Resources/Sprite/nums/8.png",
		"Resources/Sprite/nums/9.png",
	};

};

