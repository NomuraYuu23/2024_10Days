#pragma once
#include "UI.h"
class RightStickUI :
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

private: // メンバ関数

	/// <summary>
	/// 外部変数登録
	/// </summary>
	void RegisteringGlobalVariables() override;

	/// <summary>
	/// 外部変数適用
	/// </summary>
	void ApplyGlobalVariables() override;

private:

	// 移動範囲
	float movingRange_;

	// 入力があるときの色
	Vector3 inputColor_;

	// 入力がないときの色
	Vector3 notInputColor_;

	// 透明度
	float alpha_;

	// 色
	Vector4 color_;

public:

	Vector4 GetColor() { return color_; }

};

