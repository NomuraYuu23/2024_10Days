#include "Egg.h"
#include "../../../../Engine/Collision/Extrusion.h"

#include "../../Obstacle/BaseObstacle.h"
#include "../../../../Engine/3D/ModelDraw.h"
#include "../../../../Engine/Physics/Gravity.h"
#include "../../../../externals/imgui/imgui.h"
#include "../../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../Obstacle/Block/Block.h"

#include "../../../Engine/Object/BaseObjectManager.h"
#include "Bullet.h"
#include "Enemy.h"
#include "../../../Engine/Math/Ease.h"
#include "EnemyManager.h"
LevelData::MeshData Egg::EggCreate()
{

	LevelData::MeshData data;
	static size_t id;
	// 名前
	data.name = "Egg" + std::to_string(id++);
	// トランスフォーム
	data.transform = {
		1.0f,1.0f,1.0f,
		0.0f,0.0f,0.0f,
		0.0f,2.0f,0.0f
	};

	// ファイルの名前
	data.flieName = "Egg.obj";
	// ディレクトリパス
	data.directoryPath = "Resources/Model/Enemy/";
	// クラスの名前
	data.className = "Egg";
	// 親の名前
	data.parentName = "";

	// コライダー(一時的なもの、親部分はヌルにしとく)
	OBB obb;
	obb.Initialize({ 0.0f,0.0f,0.0f }, Matrix4x4::MakeIdentity4x4(), { 1.0f,2.5f,1.0f }, static_cast<Null*>(nullptr));
	data.collider = obb;

	return data;

}

void Egg::Initialize(LevelData::MeshData* data)
{


	MeshObject::Initialize(data);

	//衝突属性(自分)
	collisionAttribute_ = 0x00000100;

	// 衝突マスク(相手)
	collisionMask_ = 0xfffffeff;

	OBB obb = std::get<OBB>(*collider_.get());
	obb.SetParentObject(this);
	obb.SetCollisionAttribute(collisionAttribute_);
	obb.SetCollisionMask(collisionMask_);
	ColliderShape* colliderShape = new ColliderShape();
	*colliderShape = obb;
	collider_.reset(colliderShape);

	
	isDead_ = false;

	prePosition_ = worldTransform_.GetWorldPosition();

	// 初期設定
	material_->SetEnableLighting(BlinnPhongReflection);

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
}

void Egg::Update()
{
	MeshObject::Update();
	// 重力
	velocity_ += Gravity::Execute();
	// 速度制限
	velocity_.y = std::fmaxf(velocity_.y, -1.0f);
	// 位置更新
	worldTransform_.transform_.translate += velocity_;
	worldTransform_.UpdateMatrix();

	if (isCreateEnemy_) {
		float t = float(countUp_) / float(enemyCreateTime_);
		float color = Ease::Easing(Ease::EaseName::Lerp,1.0f,0.0f,t);
		material_->SetColor({color,color ,color ,1.0f});
		if (countUp_ < enemyCreateTime_) {
			countUp_++;
		}
		if (countUp_ == enemyCreateTime_) {
			CreateEnemy();
			isDead_ = true;
		}
	}

	// コライダー
	ColliderUpdate();

	// 速度保存
	SaveVelocityUpdate();

}

void Egg::Draw(BaseCamera& camera)
{
	MeshObject::Draw(camera);
}

void Egg::ImGuiDraw()
{

}

void Egg::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	if (std::holds_alternative<Block*>(colliderPartner)) {
		if (std::get<Block*>(colliderPartner)->GetIsAttack()) {
			isDead_ = true;
			enemyManager_->RemoveEgg(this);
		}
		OnCollisionObstacle(colliderPartner, collisionData);
	}

}

void Egg::ParticleDraw(BaseCamera& camera)
{
}


void Egg::ColliderUpdate()
{

	OBB obb = std::get<OBB>(*collider_.get());

	float coliderAddY = 1.0f;

	obb.center_ = worldTransform_.GetWorldPosition();
	obb.center_.y += obb.size_.y / 2.0f + coliderAddY;
	obb.SetOtientatuons(worldTransform_.rotateMatrix_);

	ColliderShape* colliderShape = new ColliderShape();

	*colliderShape = obb;

	collider_->swap(*colliderShape);

}


void Egg::OnCollisionObstacle(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	BaseObstacle* obstacle = std::get<Block*>(colliderPartner);

	OBB* myOBB = &std::get<OBB>(*collider_);
	OBB* partnerOBB = &std::get<OBB>(*obstacle->GetCollider());

	float myY = myOBB->center_.y - myOBB->size_.y;
	float partnerY = partnerOBB->center_.y + partnerOBB->size_.y;

	Vector3 extrusion = {};

	if (partnerY - myY <= fabsf(Gravity::Execute().y) + 0.01f) {
		extrusion.y = partnerY - myY;
	}
	else {
		extrusion = Extrusion::OBBAndOBB(myOBB, partnerOBB);
	}

	if (extrusion.y > 0.0f) {
		velocity_.y = 0.0f;
	}

	worldTransform_.transform_.translate += extrusion;
	worldTransform_.UpdateMatrix();
	// コライダー
	ColliderUpdate();
}

void Egg::CreateEnemy() {

	LevelData::ObjectData data;

	IObject* pointer = nullptr;

	data = Enemy::EnemyCreate();
	LevelData::MeshData& bullet = std::get<LevelData::MeshData>(data);
	bullet.transform.translate = worldTransform_.GetWorldPosition();
	pointer = objectManager_->AddObject(data);
	static_cast<Enemy*>(pointer)->SetPlayer(target_);
	static_cast<Enemy*>(pointer)->SetBlockManager(blockManager_);
	static_cast<Enemy*>(pointer)->SetObjectManager(objectManager_);
	enemyManager_->AddEnemy(static_cast<BaseEnemy*>(pointer));

	//自分自身をリストから除外する
	enemyManager_->RemoveEgg(this);
}

void Egg::Delete() {
	isCreateEnemy_ = false;
	isDead_ = true;
	enemyManager_->RemoveEgg(this);
}