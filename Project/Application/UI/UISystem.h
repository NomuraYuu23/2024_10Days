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

	kUITextureHandleIndexOfCount
};

enum UIIndex {
	kUIIndexLeftStickBG, // 左スティック背景
	kUIIndexLeftStick, // 左スティック
	kUIIndexLeftStickStr, // 左スティック文字
	kUIIndexButtonA, // Aボタン
	kUIIndexButtonAStr, // Aボタン文字
	kUIIndexHP1,// HP1
	kUIIndexHP0,// HP0
	kUIIndexHP2,// HP2
	kUIIndexRightStickBG, // 右スティック背景
	kUIIndexRightStick, // 右スティック
	kUIIndexRightStickStr, // 右スティック文字
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

private:

	// プレイヤー
	Player* player_;

	// テクスチャハンドル
	std::array<uint32_t, UITextureHandleIndex::kUITextureHandleIndexOfCount> textureHandles_;

	// UI
	std::array<std::unique_ptr<UI>, UIIndex::kUIIndexOfCount> UIs_;

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
	};

};

