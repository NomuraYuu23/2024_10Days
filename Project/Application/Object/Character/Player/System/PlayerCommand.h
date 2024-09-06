#pragma once
#include <cstdint>
#include "../../../../../Engine/Input/Input.h"

class Player;

class PlayerCommand
{

public:

	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns></returns>
	static PlayerCommand* GetInstance();

public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="player">プレイヤー</param>
	void Initialize(Player* player);

	/// <summary>
	/// コマンド
	/// </summary>
	/// <returns>ステート</returns>
	uint32_t Command();

private:

	static Input* input_;

	static Player* player_;

private:
	PlayerCommand() = default;
	~PlayerCommand() = default;
	PlayerCommand(const PlayerCommand&) = delete;
	const PlayerCommand& operator=(const PlayerCommand&) = delete;

};

