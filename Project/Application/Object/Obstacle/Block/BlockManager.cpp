#include "BlockManager.h"

void BlockManager::Initialize()
{

	blocks_.clear();

}

void BlockManager::AddBlock(Block* block)
{

	blocks_.push_back(block);

}
