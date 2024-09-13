#include "BlockManager.h"

void BlockManager::Initialize()
{

	blocks_.clear();

}

void BlockManager::AddBlock(Block* block)
{

	block->SetAudioManager(audioManager_);
	blocks_.push_back(block);

}

void BlockManager::Set2dArray() {
	size_t x = 0,y=0;
	for (Block* block:blocks_) {
		size_t height = size_t(block->GetHight());
		
		heights_[y][x] = height;
		x++;
		if (x>=Block::kNumOnece_) {
			x = 0;
			y++;
		}
	}
}

bool BlockManager::IsConnectRoad(Vector3 from, Vector3 to, bool hight) {

	Set2dArray();
	Vector3 length{0,0,0};
	length = Vector3::Subtract(to, from);
	if (length.x * length.x > length.z * length.z && length.x > 0.1f) { //x成分の方が長い時
		float div = std::fabsf(length.x) / Block::kSize_;
		for (size_t i = 0; i < size_t(div);i++) {
			Vector3 va = {0,0,0};
			va.x =  (from.x + length.x/div * float(i));
			va.x /= Block::kSize_*2.0f;
			va.z =  (from.z + length.z/div * float(i));
			va.z /= Block::kSize_*2.0f;
			//va.z = GetMinPoint().z + (to.z + (from.z * (length.z / Block::kSize_) * float(i)));
			va.x += float(Block::kNumOnece_) * 0.5f;
			va.z += float(Block::kNumOnece_) * 0.5f;

			if (heights_[size_t(va.z)][size_t(va.x)] != size_t(hight)) {
				return false;
			}
		}
		return true;
	}
	else if(length.z*length.z > 0.1f){
		float div = std::fabsf(length.z) / Block::kSize_;
		for (size_t i2 = 0; i2 < size_t(std::fabsf(length.z) / Block::kSize_); i2++) {
			Vector3 va = { 0,0,0 };
			va.x =  (from.x + length.x / div * float(i2));
			va.x /= Block::kSize_*2.0f;
			va.z =  (from.z + length.z / div * float(i2));
			va.z /= Block::kSize_*2.0f;
			//va.z = GetMinPoint().z + (to.z + (from.z * (length.z / Block::kSize_) * float(i)));
			va.x += float(Block::kNumOnece_) * 0.5f;
			va.z += float(Block::kNumOnece_) * 0.5f;
			if (heights_[size_t(va.z)][size_t(va.x)] != size_t(hight)) {
				return false;
			}
		}
		return true;
	}
	return false;
}

void BlockManager::AllDown() {
	for(Block* block : blocks_) {
		block->Down();
		block->SetIsRockMove(true);
	}
}