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

	/// <summary>
	/// マップの最小位置取得
	/// </summary>
	Vector3 GetMinPoint() { return { (float(Block::kNumOnece_ - 1) * 0.5f) * 2.0f * Block::kSize_,0.0f, (float(Block::kNumOnece_ - 1) * 0.5f) * 2.0f * Block::kSize_ }; };

	//ブロックの現在の状態を配列にかくのうする
	void Set2dArray();

	//平面上で二点間のブロックが全て繋がっているか

private:

	// ブロック
	std::vector<Block*> blocks_;
	
	//ブロックの高さの状態(0:下 1:上 -1:移動中)
	size_t heights_[Block::kNumOnece_][Block::kNumOnece_] = {};
};

