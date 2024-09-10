#include "FlyEnemy.h"
#include "../../../../Engine/Collision/Extrusion.h"

#include "../../Obstacle/BaseObstacle.h"
#include "../../../../Engine/3D/ModelDraw.h"
#include "../../../../Engine/Physics/Gravity.h"
#include "../../../../externals/imgui/imgui.h"
#include "../../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../Obstacle/Block/Block.h"
LevelData::MeshData FlyEnemy::FlyEnemyCreate()
{

	LevelData::MeshData data;
	static size_t id;
	// 名前
	data.name = "FlyEnemy" + std::to_string(id++);
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
	data.className = "FlyEnemy";
	// 親の名前
	data.parentName = "";

	// コライダー(一時的なもの、親部分はヌルにしとく)
	OBB obb;
	obb.Initialize({ 0.0f,0.0f,0.0f }, Matrix4x4::MakeIdentity4x4(), { 1.0f,2.5f,1.0f }, static_cast<Null*>(nullptr));
	data.collider = obb;

	return data;

}

void FlyEnemy::Initialize(LevelData::MeshData* data)
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


	RegistrationGlobalVariables();
	ApplyGlobalVariables();

	//初期ステート
	state_ = std::bind(&FlyEnemy::Move, this);
}

void FlyEnemy::Update()
{
#ifdef _DEMO
	ApplyGlobalVariables();


#endif // _DEBUG

	MeshObject::Update();

	// 位置制限
	PositionLimit();
	
	//ステート固有処理
	state_();

	worldTransform_.UpdateMatrix();

	// コライダー
	ColliderUpdate();

	// 速度保存
	SaveVelocityUpdate();

}

void FlyEnemy::Move() {
	worldTransform_.direction_ = velocity_;
	worldTransform_.transform_.translate += velocity_ * speed_;
	if (IsInnerAttackArea()) {
		//攻撃に移行
		state_ = std::bind(&FlyEnemy::RushIdle, this);
		countUp = 0;
	}
}

void FlyEnemy::RushIdle() {
	if (countUp > rushIdleLength) {
		//攻撃に移行
		state_ = std::bind(&FlyEnemy::Rush, this);
		countUp = 0;
	}
	countUp++;
}

void FlyEnemy::Rush() {
	worldTransform_.direction_ = velocity_;
	worldTransform_.transform_.translate += velocity_ * attackSpeed_;
	if (!IsInnerAttackArea()) {
		//通常移動に移行
		state_ = std::bind(&FlyEnemy::Move, this);
		countUp = 0;
	}
}

void FlyEnemy::Dead() {
	isPlayDeathAnimation_ = true;
	if (countUp > deathAnimationLength) {
		isDead_ = true;
	}
	countUp++;
}

void FlyEnemy::Draw(BaseCamera& camera)
{
	MeshObject::Draw(camera);
}

void FlyEnemy::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	if (std::holds_alternative<Block*>(colliderPartner)) {
		OnCollisionObstacle(colliderPartner, collisionData);
	}

}

void FlyEnemy::ParticleDraw(BaseCamera& camera)
{

}

void FlyEnemy::ColliderUpdate()
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


void FlyEnemy::OnCollisionObstacle(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	//BaseObstacle* obstacle = std::get<BaseObstacle*>(colliderPartner);

	//isDead_ = true;
	isPlayDeathAnimation_ = true;
}

void FlyEnemy::PositionLimit()
{

	Vector3 Max = { 64.0f,1000.0f, 64.0f };
	Vector3 Min = { -64.0f,-1000.0f, -64.0f };

	if (worldTransform_.transform_.translate.x < Min.x || Max.x < worldTransform_.transform_.translate.x ||
		worldTransform_.transform_.translate.z < Min.z || Max.z < worldTransform_.transform_.translate.z) {
		//isDead_ = true;
		if (!isPlayDeathAnimation_) {
			countUp = 0;
		}
		state_ = std::bind(&FlyEnemy::Dead, this);
	}
}

bool FlyEnemy::IsInnerAttackArea()
{

	Vector3 Max = { 18.0f,1000.0f, 18.0f };
	Vector3 Min = { -18.0f,-1000.0f, -18.0f };

	if (worldTransform_.transform_.translate.x < Min.x || Max.x < worldTransform_.transform_.translate.x ||
		worldTransform_.transform_.translate.z < Min.z || Max.z < worldTransform_.transform_.translate.z) {
		return false;
	}
	return true;
}

void FlyEnemy::ApplyGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "FlyEnemy";

	rushIdleLength = static_cast<size_t>(globalVariables->GetIntValue(groupName, "rushIdleLength"));
	deathAnimationLength = static_cast<size_t>(globalVariables->GetIntValue(groupName, "deathAnimationLength"));
	speed_ = globalVariables->GetFloatValue(groupName, "speed");
	attackSpeed_ = globalVariables->GetFloatValue(groupName, "attackSpeed");

}

void FlyEnemy::RegistrationGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "FlyEnemy";

	
	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "rushIdleLength", static_cast<int32_t>(rushIdleLength));
	globalVariables->AddItem(groupName, "deathAnimationLength", static_cast<int32_t>(deathAnimationLength));
	globalVariables->AddItem(groupName, "speed", speed_);
	globalVariables->AddItem(groupName, "attackSpeed", attackSpeed_);
}