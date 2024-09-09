#include "EnemyManager.h"
#include "Egg.h"
#include "Enemy.h"
#include "FlyEnemy.h"
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

	data.className = "FlyEnemy";
	data.spownFrame = 60;
	data.position = { -8.0f,4.0f,0.0f };
	data.velocity = { 1.0f,0,0 };
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
		if (enemy.spownFrame == frameCount_) {
			AddEnemy(enemy);
			return true;
		}
		return false;
		});

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
	else if (data.className == "FlyEnemy") {
		LevelData::ObjectData odata;

		IObject* pointer = nullptr;

		odata = FlyEnemy::FlyEnemyCreate();
		LevelData::MeshData& enemy = std::get<LevelData::MeshData>(odata);
		enemy.transform.translate = data.position;
		enemy.transform.translate.y = 16.0f;
		pointer = objectManager_->AddObject(odata);
		static_cast<FlyEnemy*>(pointer)->SetVelocity(data.velocity);
	}
}