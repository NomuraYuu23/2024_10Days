#pragma once
#include "UI.h"

class LeftStickUI;

class LeftStickBG :
    public UI
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <param name="groupName">グループ名</param>
	/// <param name="jsonName">json名</param>
	void Initialize(uint32_t textureHandle, const std::string& groupName, const std::string& jsonName = "UI") override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 親設定
	/// </summary>
	/// <param name="parent">親</param>
	void SetParent(LeftStickUI* parent);

private:

	// 親
	LeftStickUI* parent_;

};

