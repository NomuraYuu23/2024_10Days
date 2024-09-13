#include "EnemyManager.h"
#include "Egg.h"
#include "Enemy.h"
#include "FlyEnemy.h"
#include "../../../Engine/Object/BaseObjectManager.h"
#include "../../Obstacle/Block/BlockManager.h"
void EnemyManager::Initialize() {

	frameCount_ = 0;
	waveNum = 0;
	isEndAllWave_ = false;
	isBossSpawn_ = false;
	std::list<EnemyData> datas;
	EnemyData data;

	//wave1

	data.className = "Enemy";
	data.spownFrame = 180;
	data.position = {-3.0f,40.0f,3.0f};
	data.velocity = {0,0,0};
	datas.push_back(data);

	/*
	data.className = "FlyEnemy";
	data.spownFrame = 60;
	data.position = { -32.0f,4.0f,0.0f };
	data.velocity = { 1.0f,0,0 };
	datas.push_back(data);
	*/

	spownDatas_.push_back(datas);

	//wave2
	datas.clear();

	data.className = "Enemy";
	data.spownFrame = 120;
	data.position = { -21.0f,28.0f,0.0f };
	data.velocity = { 0,0,0 };
	datas.push_back(data);

	data.className = "Enemy";
	data.spownFrame = 120;
	data.position = { 21.0f,28.0f,-21.0f };
	data.velocity = { 0,0,0 };
	datas.push_back(data);

	data.className = "Enemy";
	data.spownFrame = 120;
	data.position = { 21.0f,28.0f,21.0f };
	data.velocity = { 0,0,0 };
	datas.push_back(data);

	spownDatas_.push_back(datas);

	//wave3
	datas.clear();

	data.className = "Enemy";
	data.spownFrame = 120;
	data.position = { -3.0f,28.0f,3.0f };
	data.velocity = { 0,0,0 };
	datas.push_back(data);

	data.className = "FlyEnemy";
	data.spownFrame = 180;
	data.position = { -42.0f,4.0f,18.0f };
	data.velocity = { 1.0f,0,0 };
	datas.push_back(data);

	data.className = "FlyEnemy";
	data.spownFrame = 180;
	data.position = { 42.0f,4.0f,-18.0f };
	data.velocity = { -1.0f,0,0 };
	datas.push_back(data);

	data.className = "FlyEnemy";
	data.spownFrame = 180;
	data.position = { -18.0f,24.0f,42.0f };
	data.velocity = { 0.01f,0.0f, -1.0f };
	datas.push_back(data);

	data.className = "FlyEnemy";
	data.spownFrame = 180;
	data.position = { 18.0f,24.0f,-42.0f };
	data.velocity = { 0.0f,0.0f, 1.0f };
	datas.push_back(data);


	spownDatas_.push_back(datas);

	//wave4
	datas.clear();

	data.className = "Enemy";
	data.spownFrame = 90;
	data.position = { -9.0f,28.0f,9.0f };
	data.velocity = { 0,0,0 };
	datas.push_back(data);

	data.className = "Enemy";
	data.spownFrame = 90;
	data.position = { 9.0f,28.0f,-9.0f };
	data.velocity = { 0,0,0 };
	datas.push_back(data);

	data.className = "FlyEnemy";
	data.spownFrame = 90;
	data.position = { -42.0f,4.0f,21.0f };
	data.velocity = { 1.0f,0,0 };
	datas.push_back(data);

	data.className = "FlyEnemy";
	data.spownFrame = 100;
	data.position = { -42.0f,4.0f,15.0f };
	data.velocity = { 1.0f,0,0 };
	datas.push_back(data);

	data.className = "FlyEnemy";
	data.spownFrame = 100;
	data.position = { -42.0f,4.0f,9.0f };
	data.velocity = { 1.0f,0,0 };
	datas.push_back(data);

	data.className = "FlyEnemy";
	data.spownFrame = 100;
	data.position = { -42.0f,4.0f,3.0f };
	data.velocity = { 1.0f,0,0 };
	datas.push_back(data);

	data.className = "FlyEnemy";
	data.spownFrame = 100;
	data.position = { 42.0f,24.0f,-21.0f };
	data.velocity = { -1.0f,0,0 };
	datas.push_back(data);

	data.className = "FlyEnemy";
	data.spownFrame = 100;
	data.position = { 42.0f,24.0f,-15.0f };
	data.velocity = { -1.0f,0,0 };
	datas.push_back(data);

	data.className = "FlyEnemy";
	data.spownFrame = 100;
	data.position = { 42.0f,24.0f,-9.0f };
	data.velocity = { -1.0f,0,0 };
	datas.push_back(data);

	data.className = "FlyEnemy";
	data.spownFrame = 100;
	data.position = { 42.0f,24.0f,-3.0f };
	data.velocity = { -1.0f,0,0 };
	datas.push_back(data);

	spownDatas_.push_back(datas);

	//wave5
	datas.clear();

	data.className = "Enemy";
	data.spownFrame = 90;
	data.position = { -3.0f,40.0f,3.0f };
	data.velocity = { 0,0,0 };
	datas.push_back(data);

	data.className = "FlyEnemy";
	data.spownFrame = 140;
	data.position = { 42.0f,24.0f,-3.0f };
	data.velocity = { -1.0f,0,0 };
	datas.push_back(data);

	data.className = "Enemy";
	data.spownFrame = 150;
	data.position = { 21.0f,40.0f,-3.0f };
	data.velocity = { 0,0,0 };
	datas.push_back(data);

	data.className = "Enemy";
	data.spownFrame = 150;
	data.position = { -21.0f,40.0f,-3.0f };
	data.velocity = { 0,0,0 };
	datas.push_back(data);

	data.className = "Enemy";
	data.spownFrame = 150;
	data.position = { -3.0f,40.0f,21.0f };
	data.velocity = { 0,0,0 };
	datas.push_back(data);

	data.className = "Enemy";
	data.spownFrame = 150;
	data.position = { 3f,40.0f,-21.0f };
	data.velocity = { 0,0,0 };
	datas.push_back(data);

	data.className = "FlyEnemy";
	data.spownFrame = 140;
	data.position = { -42.0f,24.0f,-3.0f };
	data.velocity = { 1.0f,0,0 };
	datas.push_back(data);

	data.className = "FlyEnemy";
	data.spownFrame = 140;
	data.position = { 42.0f,4.0f,-3.0f };
	data.velocity = { -1.0f,0,0 };
	datas.push_back(data);

	data.className = "FlyEnemy";
	data.spownFrame = 140;
	data.position = { -42.0f,4.0f,-3.0f };
	data.velocity = { 1.0f,0,0 };
	datas.push_back(data);

	data.className = "FlyEnemy";
	data.spownFrame = 150;
	data.position = { -3.0f,24.0f,-42.0f };
	data.velocity = { 0.0f,0.0f, 1.0f };
	datas.push_back(data);

	data.className = "FlyEnemy";
	data.spownFrame = 150;
	data.position = { -3.0f,24.0f,42.0f };
	data.velocity = { 0.0f,0.0f, -1.0f };
	datas.push_back(data);

	spownDatas_.push_back(datas);

	eggBreakParticleManager_ = std::make_unique<EggBreakParticleManager>();
	eggBreakParticleManager_->Initialize();

}

void EnemyManager::Update() {


	// エフェクトのリセット
	eggBreakParticleManager_->PositionClear();

	enemys_.remove_if([](BaseEnemy* enemy) {
		if (enemy->GetIsPlayDeathAnimation_()) {
			return true;
		}
		return false;
		});

	flyEnemys_.remove_if([](BaseEnemy* enemy) {
		if (enemy->GetIsPlayDeathAnimation_()) {
			return true;
		}
		return false;
		});

	spownDatas_[waveNum].remove_if([&](EnemyData enemy) {
		if (enemy.spownFrame == frameCount_) {
			AddEnemy(enemy);
			return true;
		}
		return false;
		});
	//オーバーフロー防止
	if (frameCount_ < 3600) {
		frameCount_++;
	}
	if (spownDatas_[waveNum].empty() && enemys_.empty() && eggs_.empty()) {
		if (waveNum<kWaveNum-1) {
			waveNum++;
			frameCount_ = 0;
		}
		else {
			if (!isEndAllWave_) {
				frameCount_ = 0;
			}
			isEndAllWave_ = true;
			if (frameCount_ > bossSpwanTime_) {
				isBossSpawn_ =true;
			}
			if (!isBossSpawn_) {
				frameCount_++;
			}
		}
	}

}

void EnemyManager::PostUpdate()
{

	// エフェクトの更新
	eggBreakParticleManager_->Update();

}

void EnemyManager::ParticleDraw(BaseCamera& camera)
{

	eggBreakParticleManager_->Draw(camera);

}

void EnemyManager::AddEnemy(EnemyData& data) {
	if (data.className == "Enemy") {
		LevelData::ObjectData odata;

		IObject* pointer = nullptr;

		odata = Egg::EggCreate();
		LevelData::MeshData& enemy = std::get<LevelData::MeshData>(odata);
		enemy.transform.translate = data.position;
		//enemy.transform.translate.y = 16.0f;
		pointer = objectManager_->AddObject(odata);
		static_cast<Egg*>(pointer)->SetPlayer(player_);
		static_cast<Egg*>(pointer)->SetBlockManager(blockManager_);
		static_cast<Egg*>(pointer)->SetObjectManager(objectManager_);
		//enemys_.push_back(pointer);
		static_cast<Egg*>(pointer)->SetEnemyManager(this);
		eggs_.push_back(static_cast<Egg*>(pointer));
	}
	else if (data.className == "FlyEnemy") {
		LevelData::ObjectData odata;

		IObject* pointer = nullptr;

		odata = FlyEnemy::FlyEnemyCreate();
		LevelData::MeshData& enemy = std::get<LevelData::MeshData>(odata);
		enemy.transform.translate = data.position;
		//enemy.transform.translate.y = 16.0f;
		pointer = objectManager_->AddObject(odata);
		static_cast<FlyEnemy*>(pointer)->SetVelocity(data.velocity);
		flyEnemys_.push_back(static_cast<BaseEnemy*>(pointer));

		// 音
		audioManager_->PlayWave(kGamePteraSpawnSE);

	}
}

void EnemyManager::AddEnemy(BaseEnemy* enemy){

	static_cast<Enemy*>(enemy)->SetAudioManager(audioManager_);
	enemys_.push_back(enemy);

	eggBreakParticleManager_->PositionRegister(static_cast<Enemy*>(enemy)->GetWorldTransformAdress()->GetWorldPosition());
	audioManager_->PlayWave(kGameEggBreakSE);
	audioManager_->PlayWave(kGameEnemySpawnSE);
}

void EnemyManager::AddEgg(const Vector3& position)
{
	LevelData::ObjectData odata;

	IObject* pointer = nullptr;

	odata = Egg::EggCreate();
	LevelData::MeshData& enemy = std::get<LevelData::MeshData>(odata);
	enemy.transform.translate = position;
	enemy.transform.translate.y = 28.0f;
	pointer = objectManager_->AddObject(odata);
	static_cast<Egg*>(pointer)->SetPlayer(player_);
	static_cast<Egg*>(pointer)->SetBlockManager(blockManager_);
	static_cast<Egg*>(pointer)->SetObjectManager(objectManager_);
	//enemys_.push_back(pointer);
	static_cast<Egg*>(pointer)->SetEnemyManager(this);
	static_cast<Egg*>(pointer)->SetIsCreateEnemy(false);
	eggs_.push_back(static_cast<Egg*>(pointer));
}


void EnemyManager::RemoveEgg(Egg* in) {
	eggs_.remove_if([&](Egg* egg) {
		if (egg == in) {
			return true;
		}
		return false;
		});
}