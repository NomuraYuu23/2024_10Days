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
		x++;
		if (x>=6) {
			x = 0;
			y++;
		}
	}
}

bool BlockManager::IsConnectRoad(Vector3 from, Vector3 to, bool hight) {
	Set2dArray();
	Vector3 length = to - from;
	float x = length.x;
	float z = length.z;
	if (x*x > z*z) { //x成分の方が長い時
		for (size_t i = 0; i < size_t(x / Block::kSize_);i++) {
			Vector3 va = (to + (from * (1.0f / Block::kSize_)));
			va.x += Block::kNumOnece_ * 0.5f;
			va.z += Block::kNumOnece_ * 0.5f;
			if (heights_[size_t(va.z)][size_t(va.x)] != size_t(hight)) {
				return false;
			}
		}
		return true;
	}
	else {
		for (size_t i = 0; i < size_t(z / Block::kSize_); i++) {
			Vector3 va = (to + (from * (1.0f / Block::kSize_)));
			va.x += Block::kNumOnece_ * 0.5f;
			va.z += Block::kNumOnece_ * 0.5f;
			if (heights_[size_t(va.z)][size_t(va.x)] != size_t(hight)) {
				return false;
			}
		}
		return true;
	}
	return false;
}