#pragma once
#include <vector>
#include "Block.h"

class BlockManager
{

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// ブロック追加
	/// </summary>
	/// <param name="block">ブロック</param>
	void AddBlock(Block* block);

	/// <summary>
	/// ブロック取得
	/// </summary>
	/// <returns></returns>
	std::vector<Block*>* GetBlocks() { return &blocks_; }

	/// <summary>
	/// ブロックの数
	/// </summary>
	/// <returns></returns>
	uint32_t GetBlockNum() { return static_cast<uint32_t>(blocks_.size()); }

private:

	// ブロック
	std::vector<Block*> blocks_;

};

