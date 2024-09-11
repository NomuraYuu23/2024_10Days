#include "Bullet.h"
#include "../../../../Engine/Collision/Extrusion.h"

#include "../../Obstacle/BaseObstacle.h"
#include "../../../../Engine/3D/ModelDraw.h"
#include "../../../../Engine/Physics/Gravity.h"
#include "../../../../externals/imgui/imgui.h"
#include "../../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../Obstacle/Block/Block.h"
LevelData::MeshData Bullet::BulletCreate()
{

	LevelData::MeshData data;
	static size_t id;
	// 名前
	data.name = "Bullet" + std::to_string(id++);
	// トランスフォーム
	data.transform = {
		1.0f,1.0f,1.0f,
		0.0f,0.0f,0.0f,
		0.0f,2.0f,0.0f
	};

	// ファイルの名前
	data.flieName = "bullet.obj";
	// ディレクトリパス
	data.directoryPath = "Resources/Model/Enemy/";
	// クラスの名前
	data.className = "Bullet";
	// 親の名前
	data.parentName = "";

	// コライダー(一時的なもの、親部分はヌルにしとく)
	OBB obb;
	obb.Initialize({ 0.0f,0.0f,0.0f }, Matrix4x4::MakeIdentity4x4(), { 1.0f,2.5f,1.0f }, static_cast<Null*>(nullptr));
	data.collider = obb;

	return data;

}

void Bullet::Initialize(LevelData::MeshData* data)
{


	MeshObject::Initialize(data);

	//衝突属性(自分)
	collisionAttribute_ = 0x00000200;

	// 衝突マスク(相手)
	collisionMask_ = 0xfffffdff;

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

	worldTransform_.direction_ = velocity_;
	worldTransform_.usedDirection_ = true;

}

void Bullet::Update()
{
	MeshObject::Update();

	// 位置制限
	PositionLimit();
	worldTransform_.direction_ = velocity_;
	worldTransform_.transform_.translate += velocity_ * speed_;

	worldTransform_.UpdateMatrix();

	// コライダー
	ColliderUpdate();

	// 速度保存
	SaveVelocityUpdate();

}

void Bullet::Draw(BaseCamera& camera)
{
	MeshObject::Draw(camera);
}

void Bullet::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	if (std::holds_alternative<Block*>(colliderPartner)) {
		OnCollisionObstacle(colliderPartner, collisionData);
	}
	else if (std::holds_alternative<Player*>(colliderPartner)) {
		isDead_ = true;
	}

}

void Bullet::ParticleDraw(BaseCamera& camera)
{

}

void Bullet::ColliderUpdate()
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


void Bullet::OnCollisionObstacle(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	//BaseObstacle* obstacle = std::get<BaseObstacle*>(colliderPartner);

	isDead_ = true;
}

void Bullet::PositionLimit()
{

	Vector3 Max = { 18.0f,1000.0f, 18.0f };
	Vector3 Min = { -18.0f,-1000.0f, -18.0f };

	if (worldTransform_.transform_.translate.x < Min.x || Max.x < worldTransform_.transform_.translate.x ||
		worldTransform_.transform_.translate.z < Min.z || Max.z < worldTransform_.transform_.translate.z) {
		isDead_ = true;
	}
}
