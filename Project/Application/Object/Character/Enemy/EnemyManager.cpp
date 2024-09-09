#include "EnemyManager.h"
#include "Egg.h"
#include "Enemy.h"
#include "../../../Engine/Object/BaseObjectManager.h"
#include "../../Obstacle/Block/BlockManager.h"
void EnemyManager::Initialize() {

	frameCount_ = 0;

	std::list<EnemyData> datas;
	EnemyData data;
	data.className = "Enemy";
	data.spownFrame = 180;
	data.position = {-8.0f,8.0f,16.0f};
	data.velocity = {0,0,0};
	datas.push_back(data);
	spownDatas_.push_back(datas);
}

void EnemyManager::Update() {

	enemys_.remove_if([](BaseEnemy* enemy) {
		if (enemy->GetIsPlayDeathAnimation_()) {
			return true;
		}
		return false;
		});

	spownDatas_[0].remove_if([&](EnemyData enemy) {
		if (enemy.spownFrame < frameCount_) {
			return true;
		}
		return false;
		});

	for (EnemyData& data : spownDatas_[0]){
		if (data.spownFrame == frameCount_) {
			AddEnemy(data);
		}
	}

	frameCount_++;
}

void EnemyManager::AddEnemy(EnemyData& data) {
	if (data.className == "Enemy") {
		LevelData::ObjectData odata;

		IObject* pointer = nullptr;

		odata = Egg::EggCreate();
		LevelData::MeshData& enemy = std::get<LevelData::MeshData>(odata);
		enemy.transform.translate = data.position;
		enemy.transform.translate.y = 16.0f;
		pointer = objectManager_->AddObject(odata);
		static_cast<Egg*>(pointer)->SetPlayer(player_);
		static_cast<Egg*>(pointer)->SetBlockManager(blockManager_);
		static_cast<Egg*>(pointer)->SetObjectManager(objectManager_);
	}
}