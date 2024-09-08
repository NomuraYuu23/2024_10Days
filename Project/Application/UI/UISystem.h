#pragma once
#include "../../Engine/Input/Input.h"
#include "../Object/Character/Player/Player.h"
#include "UI.h"

enum UITextureHandleIndex {
	kUITextureHandleIndexLeftStick, // 左スティック
	kUITextureHandleIndexLeftStickBG, // 左スティック背景
	kUITextureHandleIndexButtonA, // Aボタン
	kUITextureHandleIndexHP,// HP

	kUITextureHandleIndexOfCount
};

enum UIIndex {
	kUIIndexLeftStick, // 左スティック
	kUIIndexLeftStickBG, // 左スティック背景
	kUIIndexButtonA, // Aボタン
	kUIIndexHP,// HP
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

	// 操作情報
	Input* input_;

	// プレイヤー
	Player* player_;

	// テクスチャハンドル
	std::array<uint32_t, UITextureHandleIndex::kUITextureHandleIndexOfCount> textureHandles_;

	// UI
	std::array<std::unique_ptr<UI>, UIIndex::kUIIndexOfCount> UIs_;


private: 

	// テクスチャのパス
	const std::array<std::string, UITextureHandleIndex::kUITextureHandleIndexOfCount> textureNames_ = {
		"",
		"",
		"",
		"",
	};

};

