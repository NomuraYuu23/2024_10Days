#include "Enemy.h"
#include "../../../../Engine/Collision/Extrusion.h"

#include "../../Obstacle/BaseObstacle.h"
#include "../../../../Engine/3D/ModelDraw.h"
#include "../../../../Engine/Physics/Gravity.h"
#include "../../../../externals/imgui/imgui.h"
#include "../../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../Obstacle/Block/Block.h"

#include "../../../Engine/Object/BaseObjectManager.h"
#include "Bullet.h"

#include "../../../Engine/Math/Ease.h"

LevelData::MeshData Enemy::EnemyCreate()
{

	LevelData::MeshData data;
	static size_t id;
	// 名前
	data.name = "Enemy" + std::to_string(id++);
	// トランスフォーム
	data.transform = {
		1.0f,1.0f,1.0f,
		0.0f,0.0f,0.0f,
		0.0f,2.0f,0.0f
	};

	// ファイルの名前
	data.flieName = "Spino.gltf";
	// ディレクトリパス
	data.directoryPath = "Resources/Model/Enemy/";
	// クラスの名前
	data.className = "Enemy";
	// 親の名前
	data.parentName = "";

	// コライダー(一時的なもの、親部分はヌルにしとく)
	OBB obb;
	obb.Initialize({ 0.0f,0.0f,0.0f }, Matrix4x4::MakeIdentity4x4(), { 1.0f,2.5f,1.0f }, static_cast<Null*>(nullptr));
	data.collider = obb;

	return data;

}

void Enemy::Initialize(LevelData::MeshData* data)
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

	localMatrixManager_ = std::make_unique<LocalMatrixManager>();
	localMatrixManager_->Initialize(model_->GetRootNode());

	animation_.Initialize(
		model_->GetNodeAnimationData(),
		localMatrixManager_->GetInitTransform(),
		localMatrixManager_->GetNodeNames());

	// パーツ
	PartInitialize();

	// hp
	initHp_ = 3;

	runningSpeed_ = 0.3f;

	isDead_ = false;

	prePosition_ = worldTransform_.GetWorldPosition();

	// 初期設定
	material_->SetEnableLighting(BlinnPhongReflection);

	RegistrationGlobalVariables();
	ApplyGlobalVariables();

	hp_ = initHp_;

	//初期ステート
	state_ = std::bind(&Enemy::Shot, this);

	isKnockback_ = false;

	// ノックバック初めの速さ
	knockbackInitSpeed_ = 1.0f;

	// ノックバック速度
	knockbackVelocity_ = {0.0f,0.0f,0.0f};

	// ノックバック加速度
	knockbackAcceleration_ = { 0.0f,0.0f,0.0f };

	// ノックバック終了フレーム
	knockbackEndFrame_ = 10;

	// ノックバック現在のフレーム
	knockbackCountFrame_ = 0;

	// ノックバック上昇速度
	knockbackClimbSpeed_ = 0.8f;

}

void Enemy::Update()
{

#ifdef _DEMO
	ApplyGlobalVariables();


#endif // _DEBUG

	MeshObject::Update();
	if (!isPlayDeathAnimation_) {
		CheckFloorConect();
	}

	state_();

	// アニメーション
	AnimationUpdate();

	localMatrixManager_->SetNodeLocalMatrix(animation_.AnimationUpdate());

	localMatrixManager_->Map();
	if (!isPlayDeathAnimation_) {
		// 重力
		velocity_ += Gravity::Execute();
		// 速度制限
		velocity_.y = std::fmaxf(velocity_.y, -1.0f);
		// 位置更新
		worldTransform_.transform_.translate += velocity_;
		// 位置制限
		PositionLimit();
		worldTransform_.transform_.scale = oridinalScale_;
	}
	worldTransform_.UpdateMatrix();

	// コライダー
	ColliderUpdate();

	// 速度保存
	SaveVelocityUpdate();

}

void Enemy::Draw(BaseCamera& camera)
{

	ModelDraw::AnimObjectDesc desc;
	desc.camera = &camera;
	desc.localMatrixManager = localMatrixManager_.get();
	desc.material = material_.get();
	desc.model = model_;
	desc.worldTransform = &worldTransform_;
	ModelDraw::AnimObjectDraw(desc);
	
}

void Enemy::ImGuiDraw()
{

}

void Enemy::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	if (std::holds_alternative<Block*>(colliderPartner)) {
		if (std::get<Block*>(colliderPartner)->GetIsAttack() && !isPlayDeathAnimation_) {
			//死亡
			isPlayDeathAnimation_ = true;
			state_ = std::bind(&Enemy::Dead, this);
			countUp_ = 0;
		}
		OnCollisionObstacle(colliderPartner, collisionData);
	}
	else if (std::holds_alternative<Player*>(colliderPartner)) {
		if (!isKnockback_ && !isPlayDeathAnimation_) {
			KnockbackInitialize();
		}
	}

}

void Enemy::ParticleDraw(BaseCamera& camera)
{

}


void Enemy::PartInitialize()
{

	// 現在のモーション番号
	currentMotionNo_ = EnemyMotionIndex::kEnemyMotionIdle;

	// 前のモーション番号
	prevMotionNo_ = EnemyMotionIndex::kEnemyMotionIdle;

	// 待ちアニメーション
	animation_.StartAnimation(kEnemyMotionIdle, true);
	
}

void Enemy::ColliderUpdate()
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

void Enemy::AnimationUpdate()
{

	if (currentMotionNo_ != prevMotionNo_) {
		animation_.StopAnimation(prevMotionNo_);
		animation_.StartAnimation(currentMotionNo_, true);
	}

	prevMotionNo_ = currentMotionNo_;

}

void Enemy::OnCollisionObstacle(ColliderParentObject colliderPartner, const CollisionData& collisionData)
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

void Enemy::PositionLimit()
{
	//落下
	if (worldTransform_.transform_.translate.y <= -10.0f) {
		if (!isPlayDeathAnimation_) {
			//死亡
			isPlayDeathAnimation_ = true;
			state_ = std::bind(&Enemy::Dead, this);
			countUp_ = 0;
		}
	}


	Vector3 Max = { 18.0f,1000.0f, 18.0f };
	Vector3 Min = { -18.0f,-1000.0f, -18.0f };

	worldTransform_.transform_.translate.x = std::clamp(worldTransform_.transform_.translate.x, Min.x, Max.x);
	worldTransform_.transform_.translate.y = std::clamp(worldTransform_.transform_.translate.y, Min.y, Max.y);
	worldTransform_.transform_.translate.z = std::clamp(worldTransform_.transform_.translate.z, Min.z, Max.z);

}

void Enemy::ApplyGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Enemy";

	initHp_ = static_cast<uint32_t>(globalVariables->GetIntValue(groupName, "initHp"));
	runningSpeed_ = globalVariables->GetFloatValue(groupName, "runningSpeed");
	oridinalScale_ = globalVariables->GetVector3Value(groupName, "scale");
	shotFrame_ = static_cast<uint32_t>(globalVariables->GetIntValue(groupName, "shotFrame"));
	shotEnd = static_cast<uint32_t>(globalVariables->GetIntValue(groupName, "shotEnd"));
	threewayRotate_ = globalVariables->GetFloatValue(groupName, "threewayRotate");

}

void Enemy::RegistrationGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Enemy";

	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "initHp", static_cast<int32_t>(initHp_));
	globalVariables->AddItem(groupName, "runningSpeed", runningSpeed_);
	globalVariables->AddItem(groupName, "scale", worldTransform_.transform_.scale);
	globalVariables->AddItem(groupName, "shotFrame", static_cast<int32_t>(shotFrame_));
	globalVariables->AddItem(groupName, "shotEnd", static_cast<int32_t>(shotEnd));
	globalVariables->AddItem(groupName, "threewayRotate", threewayRotate_);
}

void Enemy::Idle() {
	RotateToPlayer();
	currentMotionNo_ = kEnemyMotionIdle;
}

void Enemy::Rush() {
	RotateToPlayer();
	Vector3 from = worldTransform_.GetWorldPosition();
	Vector3 to = target_->GetLastToutchBlock()->GetWorldTransformAdress()->GetWorldPosition();
	from.y = 0;
	to.y = 0;
	Vector3 move = Vector3::Normalize(to-from);

	move.y = 0;
	move *= runningSpeed_;
	worldTransform_.transform_.translate += move;
	countUp_ = shotStart_;
}

void Enemy::Shot() {
	RotateToPlayer();
	if (countUp_ == shotFrame_) {
		CreateBullet(0);
		CreateBullet(threewayRotate_);
		CreateBullet(-threewayRotate_);
	}
	currentMotionNo_ = kEnemyMotionAttack;
	if (countUp_ >= shotEnd) {
		countUp_=0;
		currentMotionNo_ = kEnemyMotionIdle;
	}
	countUp_++;
}

void Enemy::Dead() {
	currentMotionNo_ = kEnemyMotionDead;
	worldTransform_.transform_.scale = Ease::Easing(Ease::EaseName::EaseInBack, oridinalScale_, {0,0,0},float(countUp_) / float(deadEnd_));
	if (countUp_ >= deadEnd_) {
		isDead_ = true;
		countUp_ = 0;
	}
	countUp_++;
}

void Enemy::Knockback()
{

	// 減速する
	knockbackVelocity_ += knockbackAcceleration_;

	// 移動
	worldTransform_.transform_.translate = Vector3::Add(worldTransform_.transform_.translate, knockbackVelocity_);

	// 時間経過確認
	knockbackCountFrame_++;

}

void Enemy::RotateToPlayer() {
	Vector3 from = worldTransform_.GetWorldPosition();
	Vector3 to = target_->GetWorldTransformAdress()->GetWorldPosition();
	from.y = 0;
	to.y = 0;
	//プレイヤーの方を向く
	Matrix4x4 rotate = Matrix4x4::DirectionToDirection({ 0.0f,0.0f,1.0f }, to - from);
	worldTransform_.direction_ = Matrix4x4::TransformNormal({ 0.0f,0.0f,1.0f }, rotate);
	worldTransform_.usedDirection_ = true;
}

void Enemy::CheckFloorConect() {

	// ノックバック
	// 終了確認
	if (isKnockback_ && (knockbackEndFrame_ != knockbackCountFrame_)) {
		return;
	}
	isKnockback_ = false;

	bool hight = false;
	if (worldTransform_.GetWorldPosition().y > Block::kFloatHight) {
		hight = true;
	}
	Vector3 from = worldTransform_.GetWorldPosition();
	Vector3 to = target_->GetLastToutchBlock()->GetWorldTransformAdress()->GetWorldPosition();
	float lengthY = from.y - to.y;
	from.y = 0;
	to.y = 0;
	if (!(lengthY > 0 && std::fabsf(lengthY) <= 4.0f)) {
		state_ = std::bind(&Enemy::Idle, this);
		currentMotionNo_ = kEnemyMotionIdle;
		return;
	}

	if (blockManager_->IsConnectRoad(from, to, hight)) {
		//突進に移行
		state_ = std::bind(&Enemy::Rush, this);
		currentMotionNo_ = kEnemyMotionMove;
	}
	else {
		state_ = std::bind(&Enemy::Shot, this);
		currentMotionNo_ = kEnemyMotionAttack;
	}
}

void Enemy::CreateBullet(float rotateY) {

	LevelData::ObjectData data;

	IObject* pointer = nullptr;

	data = Bullet::BulletCreate();
	LevelData::MeshData& bullet = std::get<LevelData::MeshData>(data);
	bullet.transform.translate = worldTransform_.GetWorldPosition();
	bullet.transform.translate.x += worldTransform_.direction_.x * 0.5f;
	bullet.transform.translate.z += worldTransform_.direction_.z * 0.5f;
	bullet.transform.translate.y += 4.0f;
	pointer = objectManager_->AddObject(data);
	static_cast<Bullet*>(pointer)->SetVelocity(Matrix4x4::TransformNormal(Vector3::Normalize(worldTransform_.direction_),Matrix4x4::MakeRotateYMatrix(rotateY)));
	
}

void Enemy::KnockbackInitialize()
{

	state_ = std::bind(&Enemy::Knockback, this);
	countUp_ = 0;
	isKnockback_ = true;

	// 現在のフレーム
	knockbackCountFrame_ = 0;

	// 初速度設定
	Vector3 baseVelocity = { 0.0f,0.0f,knockbackInitSpeed_ };

	// 移動ベクトルをプレイヤーの角度だけ回転する
	baseVelocity = Matrix4x4::TransformNormal(baseVelocity, worldTransform_.rotateMatrix_);
	knockbackVelocity_.x = -baseVelocity.x;
	knockbackVelocity_.z = -baseVelocity.z;

	knockbackVelocity_.y = knockbackClimbSpeed_;

	// 加速度設定
	knockbackAcceleration_ = { baseVelocity.x / static_cast<float>(knockbackEndFrame_), 0.0f, baseVelocity.z / static_cast<float>(knockbackEndFrame_) };

}
