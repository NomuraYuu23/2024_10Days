#pragma once
#include "UI.h"

class Player;

class HPNumUI :
    public UI
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="player">プレイヤー</param>
	/// <param name="textureHandles">テクスチャハンドル</param>
	/// <param name="groupName">グループ名</param>
	/// <param name="jsonName">json名</param>
	void Initialize(Player* player, 
		const std::array<uint32_t, 4>& textureHandles,
		const std::string& groupName, 
		const std::string& jsonName = "UI");

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

private:

	std::array<uint32_t, 4> textureHandles_;

	Player* player_;

};

