#include "Hand.h"
#include "../../../../../Engine/Collision/Extrusion.h"

#include "../../../Obstacle/BaseObstacle.h"
#include "../../../../../Engine/3D/ModelDraw.h"
#include "../../../../../Engine/Physics/Gravity.h"
#include "../../../../../externals/imgui/imgui.h"
#include "../../../../../Engine/GlobalVariables/GlobalVariables.h"

#include "../../../Engine/Object/BaseObjectManager.h"

#include "../../../Engine/Math/Ease.h"

LevelData::MeshData Hand::HandCreate(size_t direction)
{

	LevelData::MeshData data;
	static size_t id;
	// 名前
	data.name = "Hand" + std::to_string(id++);
	// トランスフォーム
	data.transform = {
		1.0f,1.0f,1.0f,
		0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f
	};

	// ファイルの名前
	data.flieName = "Hand.obj";
	// ディレクトリパス
	data.directoryPath = "Resources/Model/Boss/";
	// クラスの名前
	data.className = "Hand";
	// 親の名前
	data.parentName = "";

	//
	data.transform.rotate.x = float(direction);

	// コライダー(一時的なもの、親部分はヌルにしとく)
	OBB obb;
	obb.Initialize({ 0.0f,0.0f,0.0f }, Matrix4x4::MakeIdentity4x4(), { 1.0f,2.5f,1.0f }, static_cast<Null*>(nullptr));
	data.collider = obb;

	return data;

}

void Hand::Initialize(LevelData::MeshData* data)
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

	// hp
	initHp_ = 3;

	isDead_ = false;

	direction_ = size_t(worldTransform_.transform_.rotate.x);
	worldTransform_.transform_.rotate.x = 0;

	prePosition_ = worldTransform_.GetWorldPosition();

	// 初期設定
	material_->SetEnableLighting(BlinnPhongReflection);

	//RegistrationGlobalVariables();
	//ApplyGlobalVariables();

	hp_ = initHp_;

	//初期ステート
	state_ = std::bind(&Hand::Root, this);

}

void Hand::Update()
{

#ifdef _DEMO
	//ApplyGlobalVariables();


#endif // _DEBUG

	MeshObject::Update();
	
	state_();

	worldTransform_.UpdateMatrix();

	// コライダー
	ColliderUpdate();

	isCollision_ = false;
}

void Hand::Draw(BaseCamera& camera)
{

	MeshObject::Draw(camera);

}

void Hand::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	if (std::holds_alternative<Block*>(colliderPartner)) {
		if (std::get<Block*>(colliderPartner)->GetIsAttack() && !isHitCoolTime_) {
			hp_--;
			//state_ = std::bind(&Hand::Damage, this);
			countUp_ = 0;
		}
		OnCollisionObstacle(colliderPartner, collisionData);
	}

}

void Hand::ParticleDraw(BaseCamera& camera)
{

}


void Hand::ColliderUpdate()
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


void Hand::OnCollisionObstacle(ColliderParentObject colliderPartner, const CollisionData& collisionData)
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

	isCollision_ = true;
}

/*
void Hand::ApplyGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Hand";

	initHp_ = static_cast<uint32_t>(globalVariables->GetIntValue(groupName, "initHp"));
	runningSpeed_ = globalVariables->GetFloatValue(groupName, "runningSpeed");
	oridinalScale_ = globalVariables->GetVector3Value(groupName, "scale");
	shotFrame_ = static_cast<uint32_t>(globalVariables->GetIntValue(groupName, "shotFrame"));
	threewayRotate_ = globalVariables->GetFloatValue(groupName, "threewayRotate");

}

void Hand::RegistrationGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Hand";

	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "initHp", static_cast<int32_t>(initHp_));
	globalVariables->AddItem(groupName, "runningSpeed", runningSpeed_);
	globalVariables->AddItem(groupName, "scale", worldTransform_.transform_.scale);
	globalVariables->AddItem(groupName, "shotFrame", static_cast<int32_t>(shotFrame_));
	globalVariables->AddItem(groupName, "threewayRotate", threewayRotate_);
}
*/

void Hand::Root() {
	worldTransform_.transform_.translate = {0,0,0};

}

void Hand::StampStand() {

}


void Hand::StampAttack() {

}