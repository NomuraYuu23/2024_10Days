#pragma once
#include "UI.h"

class Player;

class HPUI :
    public UI
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="player">プレイヤー</param>
	/// <param name="displayValue">表示する体力</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <param name="groupName">グループ</param>
	/// <param name="jsonName">json</param>
	void Initialize(
		Player* player,
		uint32_t displayValue,
		uint32_t textureHandle,
		const std::string& groupName, 
		const std::string& jsonName = "UI");

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

	// プレイヤー
	Player* player_;

	// 表示する体力
	int32_t displayValue_;

	// ベース位置
	static Vector2 basePosition_;

	// 間隔
	static float space_;

};

