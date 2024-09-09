#pragma once
#include "../../Engine/Object/AbstractObjectFactory.h"
#include "../../Engine/Level/LevelData.h"
#include "../../Engine/Object/BaseObjectManager.h"

class ObjectFactory :
    public AbstractObjectFactory
{

public: // サブクラス,定数

	enum CreateObjectIndex {
		kCreateObjectIndexGround, // 地面
		kCreateObjectIndexPlayer, // プレイヤー
		kCreateObjectIndexBlock,//ブロック
		kCreateObjectIndexPlayerHorn, //プレイヤーの角
		kCreateObjectIndexEnemy,//敵
		kCreateObjectIndexBullet,//弾
		kCreateObjectIndexEgg,//敵のタマゴ
		kCreateObjectIndexOfCount, // 数数える用
	};

public: // 静的メンバ関数

	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns></returns>
	static ObjectFactory* GetInstance();

public: //メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="objectManager">オブジェクトマネージャー</param>
	/// <param name="bossSystem">ボスシステム</param>
	void Initialize(BaseObjectManager* objectManager);

	/// <summary>
	/// オブジェクト作成
	/// </summary>
	/// <param name="objectData">オブジェクトデータ/param>
	/// <returns>オブジェクト</returns>
	IObject* CreateObject(LevelData::ObjectData& objectData) override;

	/// <summary>
	/// オブジェクトマネージャー設定
	/// </summary>
	/// <param name="objectManager"></param>
	void SetObjectManager(BaseObjectManager* objectManager) { objectManager_ = objectManager; }

private: // メンバ関数

	/// <summary>
	/// 地面
	/// </summary>
	/// <param name="objectData">オブジェクトデータ</param>
	/// <returns>オブジェクト</returns>
	static IObject* CreateObjectGround(LevelData::ObjectData& objectData);

	/// <summary>
	/// プレイヤー
	/// </summary>
	/// <param name="objectData">オブジェクトデータ</param>
	/// <returns>オブジェクト</returns>
	static IObject* CreateObjectPlayer(LevelData::ObjectData& objectData);

	/// <summary>
	/// ブロック
	/// </summary>
	/// <param name="objectData">オブジェクトデータ</param>
	/// <returns>オブジェクト</returns>
	static IObject* CreateObjectBlock(LevelData::ObjectData& objectData);

	/// <summary>
	/// プレイヤーの角
	/// </summary>
	/// <param name="objectData">オブジェクトデータ</param>
	/// <returns>オブジェクト</returns>
	static IObject* CreateObjectPlayerHorn(LevelData::ObjectData& objectData);

	/// <summary>
	/// 敵
	/// </summary>
	/// <param name="objectData">オブジェクトデータ</param>
	/// <returns>オブジェクト</returns>
	static IObject* CreateObjectEnemy(LevelData::ObjectData& objectData);

	/// <summary>
	/// 弾
	/// </summary>
	/// <param name="objectData">オブジェクトデータ</param>
	/// <returns>オブジェクト</returns>
	static IObject* CreateObjectBullet(LevelData::ObjectData& objectData);

	/// <summary>
	/// タマゴ
	/// </summary>
	/// <param name="objectData">オブジェクトデータ</param>
	/// <returns>オブジェクト</returns>
	static IObject* CreateObjectEgg(LevelData::ObjectData& objectData);

private:

	// オブジェクト作成でそれぞれのタイプを作成するための関数群
	// 返り値 無し
	// 引数1 オブジェクト
	static std::array<
		std::pair<std::string,std::function<IObject* (LevelData::ObjectData&)>>,
		CreateObjectIndex::kCreateObjectIndexOfCount> createObjectFunctions_;

	// マネージャー
	static BaseObjectManager* objectManager_;

private:
	ObjectFactory() = default;
	~ObjectFactory() = default;
	ObjectFactory(const ObjectFactory&) = delete;
	const ObjectFactory& operator=(const ObjectFactory&) = delete;

};

