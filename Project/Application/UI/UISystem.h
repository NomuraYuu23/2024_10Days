#pragma once
#include "../../Engine/Input/Input.h"
#include "../Object/Character/Player/Player.h"
#include "UI.h"

enum UITextureHandleIndex {
	kUITextureHandleIndexLeftStickBG, // 左スティック背景
	kUITextureHandleIndexLeftStick, // 左スティック
	kUITextureHandleIndexLeftStickStr, // 左スティック文字
	kUITextureHandleIndexButtonA, // Aボタン
	kUITextureHandleIndexButtonAStr, // Aボタン文字
	//kUITextureHandleIndexHP,// HP

	kUITextureHandleIndexOfCount
};

enum UIIndex {
	kUIIndexLeftStickBG, // 左スティック背景
	kUIIndexLeftStick, // 左スティック
	kUIIndexLeftStickStr, // 左スティック文字
	kUIIndexButtonA, // Aボタン
	kUIIndexButtonAStr, // Aボタン文字
	//kUIIndexHP,// HP
	kUIIndexOfCount
};

class UISystem
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="dxCommon"></param>
	void Initialize(DirectXCommon* dxCommon);

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
		"Resources/UI/joystick_L_BG.png",
		"Resources/UI/joystick_L.png",
		"Resources/UI/moveUI.png",
		"Resources/UI/buttonA.png",
		"Resources/UI/jumpUI.png",
		//"",
	};

};

