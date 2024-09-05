#pragma once
#include <cstdint>
#include "../../../../../Engine/Input/Input.h"
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
	void Initialize();

	/// <summary>
	/// コマンド
	/// </summary>
	/// <returns>ステート</returns>
	uint32_t Command();

private:

	static Input* input_;

private:
	PlayerCommand() = default;
	~PlayerCommand() = default;
	PlayerCommand(const PlayerCommand&) = delete;
	const PlayerCommand& operator=(const PlayerCommand&) = delete;

};

