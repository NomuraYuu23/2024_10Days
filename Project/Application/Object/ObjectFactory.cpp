#include "ObjectFactory.h"
#include "../../Engine/Object/MeshObject.h"

#include "../Object/Obstacle/Ground/Ground.h"
#include "../Object/Character/Player/Player.h"
#include "../Object/Obstacle/Block/Block.h"
#include "../Object/Character/Player/Horn/PlayerHorn.h"
#include "../Object/Character/Enemy/Enemy.h"
#include "../Object/Character/Enemy/Bullet.h"
#include "../Object/Character/Enemy/Egg.h"
#include "../Object/Character/Enemy/FlyEnemy.h"

// オブジェクト作成でそれぞれのタイプを作成するための関数群
// 返り値 無し
// 引数1 オブジェクト
std::array<
	std::pair<std::string, std::function<IObject*(LevelData::ObjectData&)>>,
	ObjectFactory::CreateObjectIndex::kCreateObjectIndexOfCount> ObjectFactory::createObjectFunctions_ = {};

// マネージャー
BaseObjectManager* ObjectFactory::objectManager_ = nullptr;

ObjectFactory* ObjectFactory::GetInstance()
{
	static ObjectFactory instance;
	return &instance;
}

void ObjectFactory::Initialize(BaseObjectManager* objectManager)
{

	objectManager_ = objectManager;

	createObjectFunctions_[kCreateObjectIndexGround].first = "Ground";
	createObjectFunctions_[kCreateObjectIndexGround].second = ObjectFactory::CreateObjectGround;

	createObjectFunctions_[kCreateObjectIndexPlayer].first = "Player";
	createObjectFunctions_[kCreateObjectIndexPlayer].second = ObjectFactory::CreateObjectPlayer;

	createObjectFunctions_[kCreateObjectIndexBlock].first = "Block";
	createObjectFunctions_[kCreateObjectIndexBlock].second = ObjectFactory::CreateObjectBlock;


	createObjectFunctions_[kCreateObjectIndexPlayerHorn].first = "PlayerHorn";
	createObjectFunctions_[kCreateObjectIndexPlayerHorn].second = ObjectFactory::CreateObjectPlayerHorn;

	createObjectFunctions_[kCreateObjectIndexEnemy].first = "Enemy";
	createObjectFunctions_[kCreateObjectIndexEnemy].second = ObjectFactory::CreateObjectEnemy;

	createObjectFunctions_[kCreateObjectIndexBullet].first = "Bullet";
	createObjectFunctions_[kCreateObjectIndexBullet].second = ObjectFactory::CreateObjectBullet;

	createObjectFunctions_[kCreateObjectIndexEgg].first = "Egg";
	createObjectFunctions_[kCreateObjectIndexEgg].second = ObjectFactory::CreateObjectEgg;

	createObjectFunctions_[kCreateObjectIndexFlyEnemy].first = "FlyEnemy";
	createObjectFunctions_[kCreateObjectIndexFlyEnemy].second = ObjectFactory::CreateObjectFlyEnemy;
}

IObject* ObjectFactory::CreateObject(LevelData::ObjectData& objectData)
{

	IObject* object = nullptr;

	// 確認のためメッシュオブジェクトのみ
	// クラスの名前など取得してオブジェクトを作る
	if (std::holds_alternative<LevelData::MeshData>(objectData)) {

		LevelData::MeshData data = std::get<LevelData::MeshData>(objectData);

		for (uint32_t i = 0; i < kCreateObjectIndexOfCount; ++i) {
			if (data.className == createObjectFunctions_[i].first) {
				object = createObjectFunctions_[i].second(objectData);
			}

		}

	}

    return object;

}

IObject* ObjectFactory::CreateObjectGround(LevelData::ObjectData& objectData)
{
	// インスタンス生成
	IObject* object = new Ground();
	// 初期化
	static_cast<Ground*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
	return object;
}

IObject* ObjectFactory::CreateObjectPlayer(LevelData::ObjectData& objectData)
{
	// インスタンス生成
	IObject* object = new Player();
	// 初期化
	static_cast<Player*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
	return object;
}

IObject* ObjectFactory::CreateObjectBlock(LevelData::ObjectData& objectData)
{
	// インスタンス生成
	IObject* object = new Block();
	// 初期化
	static_cast<Block*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
	return object;
}

IObject* ObjectFactory::CreateObjectPlayerHorn(LevelData::ObjectData& objectData)
{
	// インスタンス生成
	IObject* object = new PlayerHorn();
	// 初期化
	static_cast<PlayerHorn*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
	return object;
}

IObject* ObjectFactory::CreateObjectEnemy(LevelData::ObjectData& objectData)
{
	// インスタンス生成
	IObject* object = new Enemy();
	// 初期化
	static_cast<Enemy*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
	return object;
}

IObject* ObjectFactory::CreateObjectBullet(LevelData::ObjectData& objectData)
{
	// インスタンス生成
	IObject* object = new Bullet();
	// 初期化
	static_cast<Bullet*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
	return object;
}

IObject* ObjectFactory::CreateObjectEgg(LevelData::ObjectData& objectData)
{
	// インスタンス生成
	IObject* object = new Egg();
	// 初期化
	static_cast<Bullet*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
	return object;
}

IObject* ObjectFactory::CreateObjectFlyEnemy(LevelData::ObjectData& objectData)
{
	// インスタンス生成
	IObject* object = new FlyEnemy();
	// 初期化
	static_cast<Enemy*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
	return object;
}