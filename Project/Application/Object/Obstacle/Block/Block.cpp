#include "Block.h"
#include "../../../Engine/Math/Ease.h"
#include "../../Character/Player/Player.h"

#include <variant>
const float Block::kSize_ = 3.0f;
const float Block::kFloatHight = 21.0f;
const float Block::kLowHight = -2.0f;

const Vector3 Block::kMinRange_ = {-Block::kSize_ * float(Block::kNumOnece_),-1000,-Block::kSize_ * float(Block::kNumOnece_) };
const Vector3 Block::kMaxRange_ = { Block::kSize_ * float(Block::kNumOnece_), 1000, Block::kSize_ * float(Block::kNumOnece_) };

LevelData::MeshData Block::BlockCreate() {
	LevelData::MeshData data;
	static size_t id;
	// 名前
	data.name = "Block" + std::to_string(id++);
	// トランスフォーム
	data.transform = {
		kSize_,kSize_,kSize_,
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

	//衝撃波
	shockWave_.reset(new ShockWave);
	ShockWaveColliderInitialize();
}

void Block::ShockWaveColliderInitialize() {
	//衝突属性(自分)
	uint32_t collisionAttribute = 0x00000020;

	// 衝突マスク(相手)
	uint32_t collisionMask = 0xffffffde;

	ColliderShape* colliderShape1 = new ColliderShape();
	OBB obbs;
	obbs.Initialize({ 0.0f,0.0f,0.0f }, Matrix4x4::MakeIdentity4x4(), { 1.0f,1.0f,1.0f }, static_cast<Null*>(nullptr));
	*colliderShape1 = obbs;
	shockWaveCollider_.reset(colliderShape1);

	OBB obb = std::get<OBB>(*shockWaveCollider_.get());
	obb.SetParentObject(shockWave_.get());
	obb.SetCollisionAttribute(collisionAttribute);
	obb.SetCollisionMask(collisionMask);
	ColliderShape* colliderShape = new ColliderShape();
	*colliderShape = obb;
	shockWaveCollider_.reset(colliderShape);
}

void Block::ShockWaveColliderUpdate() {
	OBB obb = std::get<OBB>(*shockWaveCollider_.get());

	obb.center_ = worldTransform_.GetWorldPosition();
	obb.size_ = worldTransform_.transform_.scale;
	obb.size_.x *= 1.1f;
	obb.size_.z *= 1.1f;

	obb.SetOtientatuons(worldTransform_.rotateMatrix_);

	ColliderShape* colliderShape = new ColliderShape();

	*colliderShape = obb;

	shockWaveCollider_.reset(colliderShape);
}

void Block::Update() {
	BaseObstacle::Update();
	worldTransform_.UpdateMatrix();
	state_();
	isCollision_ = false;

	//色変更
	colorCount_++;
	float t = float(colorCount_) / float(colorLength_);
	Vector3 color = Ease::Easing(Ease::EaseName::Lerp,{1.0f,1.0f,1.0f}, blownColor_, t);
	material_->SetColor({ color.x,color.y,color.z,1.0f });
	if (colorCount_ > colorLength_) {
		colorCount_ = colorLength_;
	}

	ColliderUpdate();
	ShockWaveColliderUpdate();
}


void Block::CollisionListRegister(CollisionManager* collisionManager)
{
	BaseObstacle::CollisionListRegister(collisionManager);
	if (!isShockWaveCollision_) {
		return;
	}

	collisionManager->ListRegister(shockWaveCollider_.get());

}

void Block::CollisionListRegister(CollisionManager* collisionManager, ColliderDebugDraw* colliderDebugDraw)
{
	BaseObstacle::CollisionListRegister(collisionManager, colliderDebugDraw);
	if (!isShockWaveCollision_) {
		return;
	}

	collisionManager->ListRegister(shockWaveCollider_.get());

	colliderDebugDraw->AddCollider(*shockWaveCollider_.get());

}

void Block::ColliderUpdate()
{

	OBB obb = std::get<OBB>(*collider_.get());

	obb.center_ = worldTransform_.GetWorldPosition();
	obb.size_ = worldTransform_.transform_.scale;
	obb.SetOtientatuons(worldTransform_.rotateMatrix_);

	ColliderShape* colliderShape = new ColliderShape();

	*colliderShape = obb;

	collider_.reset(colliderShape);

}

void Block::Draw(BaseCamera& camera) {
	BaseObstacle::Draw(camera);
}

void Block::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData) {
	//待機状態かつプレイヤーとぶつかったら
	if ( !isAttack_ && !isShockWave_ && std::holds_alternative<Player*>(colliderPartner)) {
		float velocity = std::get<Player*>(colliderPartner)->GetWorldTransformAdress()->GetWorldPosition().y - worldTransform_.GetWorldPosition().y;
		if (velocity > 0) {//プレイヤーが上からぶつかったら移動
			if (std::get<Player*>(colliderPartner)->GetCurrentStateNo() == PlayerState::kPlayerStateHeadDrop) {//プレイヤーが落下攻撃状態だったら
				countUp_ = 0;
				state_ = std::bind(&Block::ShockWaveCenter, this);
				isShockWave_ = true;
			}
			else{
				colorCount_ = 0;
				isCollision_ = true;
				isMove_ = true;
			}
			//countUp_ = 0;
			//state_ = std::bind(&Block::ShockWaveCenter, this);
			//isShockWave_ = true;
		}
		else if(velocity < 1.0f && !isMoveNow_ && !std::get<Player*>(colliderPartner)->GetIsKnockBlock()){//プレイヤーが下からぶつかったら攻撃
			AttackStart();
			isMove_ = false;
			std::get<Player*>(colliderPartner)->SetIsKnockBlock(true);
		}
		
	}

	//衝撃波判定とぶつかったら
	if (!isAttack_ &&  !isShockWave_ && std::holds_alternative<ShockWave*>(colliderPartner)) {
		countUp_ = 0;
		state_ = std::bind(&Block::ShockWaveAfter, this);
		isShockWave_ = true;
	}
}

void Block::Idle() {
	if (isMove_ && !isCollision_) {
		MoveStart();
		return;
	}
}

void Block::MoveStart() {
	if (!isRockMove_) {
		countUp_ = 0;
		state_ = std::bind(&Block::Move, this);
		audioManager_->PlayWave(kGameBlockSE);
	}
}

void Block::Up() {
	if (!hight_) {
		if (!isMoveNow_) {
			countUp_ = 0;
			audioManager_->PlayWave(kGameBlockSE);
		}
		state_ = std::bind(&Block::Move, this);
	}
}

void Block::Down() {
	if (hight_) {
		if (!isMoveNow_) {
			countUp_ = 0;
			audioManager_->PlayWave(kGameBlockSE);
		}
		state_ = std::bind(&Block::Move, this);
	}
}


void Block::Move() {
	isMoveNow_ = true;
	//イージングで移動
	float t = float(countUp_) / float(moveAnimationLength_);
	Vector3 to = initialPosition_;
	Vector3 from = initialPosition_;
	to.y += float(!hight_) * floatHight_;
	from.y += float(hight_) * floatHight_;
	worldTransform_.transform_.translate  = Ease::Easing(Ease::EaseName::EaseInQuad,from,to,t);
	
	//移動終了したら待機状態にもどる
	if (countUp_ >= moveAnimationLength_) {
		isMoveNow_ = false;
		isMove_ = false;
		countUp_ = 0;
		hight_ = !hight_;
		state_ = std::bind(&Block::Idle, this);
		return;
	}
	countUp_++;
}

void Block::AttackStart() {
	countUp_ = 0;
	state_ = std::bind(&Block::Attack, this);
	isAttack_ = true;
}

void Block::Attack() {
	//イージングで移動
	float t = float(countUp_) / float(attackAnimationLength_);
	worldTransform_.transform_.rotate.x = Ease::Easing(Ease::EaseName::EaseOutQuint, 0, 3.141592f * 2.0f, t);
	Vector3 to = initialPosition_;
	Vector3 from = initialPosition_;
	to.y += float(hight_) * floatHight_ + attackFloatStrength_;
	from.y += float(hight_) * floatHight_;
	t = (2.0f * t) - 1.0f;
	t = -t * -t + 1.0f;
	worldTransform_.transform_.translate = Ease::Easing(Ease::EaseName::EaseOutQuad, from, to, t);
	//移動終了したら待機状態にもどる
	if (countUp_ >= attackAnimationLength_) {
		//worldTransform_.transform_.translate.y = float(hight_) * floatHight_;
		countUp_ = 0;
		isAttack_ = false;
		state_ = std::bind(&Block::Idle, this);
		return;
	}
	countUp_++;
}

void Block::ShockWaveCenter() {
	//イージングで移動
	float t = float(countUp_) / float(30);
	Vector3 to = initialPosition_;
	Vector3 from = initialPosition_;
	to.y += float(hight_) * floatHight_ - 1.5f;
	from.y += float(hight_) * floatHight_;
	t = (2.0f * t) - 1.0f;
	t = -t * -t + 1.0f;
	worldTransform_.transform_.translate = Ease::Easing(Ease::EaseName::EaseOutQuad, from, to, t);

	isShockWaveCollision_ = false;
	if (countUp_ == 5) {
		isShockWaveCollision_ = true;
	}

	//移動終了したら待機状態にもどる
	if (countUp_ >= 30) {
		//worldTransform_.transform_.translate.y = float(hight_) * floatHight_;
		countUp_ = 0;
		isShockWave_ = false;
		state_ = std::bind(&Block::Idle, this);
		return;
	}
	countUp_++;
}

void Block::ShockWaveAfter() {
	//イージングで移動
	float t = float(countUp_) / float(attackAnimationLength_);
	worldTransform_.transform_.rotate.x = Ease::Easing(Ease::EaseName::EaseOutQuint, 0, 3.141592f * 2.0f, t);
	Vector3 to = initialPosition_;
	Vector3 from = initialPosition_;
	to.y += float(hight_) * floatHight_ + attackFloatStrength_;
	from.y += float(hight_) * floatHight_;
	t = (2.0f * t) - 1.0f;
	t = -t * -t + 1.0f;
	worldTransform_.transform_.translate = Ease::Easing(Ease::EaseName::EaseOutQuad, from, to, t);
	//移動終了したら待機状態にもどる
	if (countUp_ >= attackAnimationLength_) {
		//worldTransform_.transform_.translate.y = float(hight_) * floatHight_;
		countUp_ = 0;
		isShockWave_ = false;
		state_ = std::bind(&Block::Idle, this);
		return;
	}
	countUp_++;
}