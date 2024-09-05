#include "Block.h"
#include "../../../Engine/Math/Ease.h"

LevelData::MeshData Block::BlockCreate() {
	LevelData::MeshData data;
	static size_t id;
	// 名前
	data.name = "Block" + id;
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
	state_ = std::bind(&Block::Idle,this);
	isMove_ = false;
	isCollision_ = false;
}


void Block::Update() {
	state_();
	isCollision_ = false;
}

void Block::Draw(BaseCamera& camera) {
	BaseObstacle::Draw(camera);
}

void Block::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData) {
	isCollision_ = true;
	isMove_ = true;
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
	Vector3 to = worldTransform_.transform_.translate;
	Vector3 from = worldTransform_.transform_.translate;
	to.y = float(!hight_);
	from.y = float(hight_);
	worldTransform_.transform_.translate  = Ease::Easing(Ease::EaseName::EaseInQuad,from,to,t);
	
	//移動終了したら待機状態にもどる
	if (countUp_ >= moveAnimationLength_) {
		countUp_ = 0;
		hight_ = !hight_;
		state_ = std::bind(&Block::Idle, this);
		return;
	}
	countUp_++;
}