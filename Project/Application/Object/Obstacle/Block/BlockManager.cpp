#include "BlockManager.h"

void BlockManager::Initialize()
{

	blocks_.clear();

}

void BlockManager::AddBlock(Block* block)
{

	blocks_.push_back(block);

}

void BlockManager::Set2dArray() {
	size_t x = 0,y=0;
	for (Block* block:blocks_) {
		size_t height = size_t(block->GetHight());
		
		heights_[y][x] = height;
	}
}