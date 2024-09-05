#include "Block.h"
#include "../../../Engine/Math/Ease.h"
#include "../../Character/Player/Player.h"

#include <variant>

LevelData::MeshData Block::BlockCreate() {
	LevelData::MeshData data;
	static size_t id;
	// 名前
	data.name = "Block" + std::to_string(id++);
	// トランスフォーム
	data.transform = {
		1.0f,1.0f,1.0f,
		0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f
	};

	// ファイルの名前
	data.flieName = "block.obj";
	// ディレクトリパス
	data.directoryPath = "Resources/Model/FloatBlock/";
	// クラスの名前
	data.className = "Block";
	// 親の名前
	data.parentName = "";

	// コライダー(一時的なもの、親部分はヌルにしとく)
	OBB obb;
	obb.Initialize({ 0.0f,0.0f,0.0f }, Matrix4x4::MakeIdentity4x4(), { 1.0f,1.0f,1.0f }, static_cast<Null*>(nullptr));
	data.collider = obb;

	return data;
}

void Block::Initialize(LevelData::MeshData* data)
{

	BaseObstacle::Initialize(data);

	//衝突属性(自分)
	collisionAttribute_ = 0x00000010;

	// 衝突マスク(相手)
	collisionMask_ = 0xffffffef;

	OBB obb = std::get<OBB>(*collider_.get());
	obb.SetParentObject(this);
	obb.SetCollisionAttribute(collisionAttribute_);
	obb.SetCollisionMask(collisionMask_);
	ColliderShape* colliderShape = new ColliderShape();
	*colliderShape = obb;
	collider_.reset(colliderShape);

	state_ = std::bind(&Block::Idle,this);
	isMove_ = false;
	isCollision_ = false;
	initialPosition_ = worldTransform_.transform_.translate;
}


void Block::Update() {
	BaseObstacle::Update();
	worldTransform_.UpdateMatrix();
	state_();
	isCollision_ = false;

	ColliderUpdate();
}

void Block::ColliderUpdate()
{

	OBB obb = std::get<OBB>(*collider_.get());

	obb.center_ = worldTransform_.GetWorldPosition();
	obb.SetOtientatuons(worldTransform_.rotateMatrix_);

	ColliderShape* colliderShape = new ColliderShape();

	*colliderShape = obb;

	collider_.reset(colliderShape);

}

void Block::Draw(BaseCamera& camera) {
	BaseObstacle::Draw(camera);
}

void Block::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData) {
	if (std::holds_alternative<Player*>(colliderPartner)) {
		//衝突相手がプレイヤーだったら移動フラグをたてる
		isCollision_ = true;
		isMove_ = true;
	}

}

void Block::Idle() {
	if (isMove_ && !isCollision_) {
		MoveStart();
		return;
	}
}

void Block::MoveStart() {
	countUp_ = 0;
	state_ = std::bind(&Block::Move, this);
}

void Block::Move() {
	//イージングで移動
	float t = float(countUp_) / float(moveAnimationLength_);
	Vector3 to = initialPosition_;
	Vector3 from = initialPosition_;
	to.y += float(!hight_) * 8.0f ;
	from.y += float(hight_) * 8.0f ;
	worldTransform_.transform_.translate  = Ease::Easing(Ease::EaseName::EaseInQuad,from,to,t);
	
	//移動終了したら待機状態にもどる
	if (countUp_ >= moveAnimationLength_) {
		isMove_ = false;
		countUp_ = 0;
		hight_ = !hight_;
		state_ = std::bind(&Block::Idle, this);
		return;
	}
	countUp_++;
}