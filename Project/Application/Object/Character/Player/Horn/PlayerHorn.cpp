#include "PlayerHorn.h"

#include "../Player.h"

LevelData::MeshData PlayerHorn::PlayerHornCreate(const std::string& name)
{

	LevelData::MeshData data;

	// 名前
	data.name = name;
	// トランスフォーム
	data.transform = {
		3.0f,3.0f,3.0f,
		0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f
	};

	// ファイルの名前
	data.flieName = "Horn.obj";
	// ディレクトリパス
	data.directoryPath = "Resources/Model/Player/";
	// クラスの名前
	data.className = "PlayerHorn";
	// 親の名前
	data.parentName = "Player";

	// コライダー(一時的なもの、親部分はヌルにしとく)
	OBB obb;
	obb.Initialize({ 0.0f,0.0f,0.0f }, Matrix4x4::MakeIdentity4x4(), { 1.0f,1.0f,1.0f }, static_cast<Null*>(nullptr));
	data.collider = obb;

	return data;

}

void PlayerHorn::Initialize(LevelData::MeshData* data)
{

	MeshObject::Initialize(data);

	// コライダーの初期化
	ColliderInitialize();

	// 剛体の初期化
	//RigidBodyInitialize();

	// 抵抗
	//coefficientOfRestitution = 0.8f;

	// 初期設定
	material_->SetEnableLighting(BlinnPhongReflection);

	// 回転
	localRotate_ = { 0.0f, 0.0f, 0.0f };
	
	// 位置
	localPosition_ = { 0.0f, -0.2f, -0.02f };

	// 大きさ
	localScale_ = { 1.0f,1.0f,1.0f };

}

void PlayerHorn::Update()
{

	MeshObject::Update();

	// 親あり更新
	if (parentMatrix_) {
		WithParentsUpdate();
	}
	// 親なし更新
	else {
		ParentlessUpdate();
	}

	// コライダーの更新
	ColliderUpdate();

	// 速度保存
	SaveVelocityUpdate();

}

void PlayerHorn::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	return;

}

void PlayerHorn::SetParent(Player* parent, const std::string& parentName)
{

	parent_ = parent;

	LocalMatrixManager* localMatrixManager = parent_->GetLocalMatrixManager();

	std::vector<std::string> names = localMatrixManager->GetNodeNames();

	for (uint32_t i = 0; i < names.size(); ++i) {
		if (parentName == names[i]) {
			parentNodeData_ = &localMatrixManager->GetNodeDatasAddress()->at(i);
			parentMatrix_ = &parent_->GetWorldTransformAdress()->parentMatrix_;
			break;
		}
	}

}

void PlayerHorn::WithParentsUpdate()
{

	// ノード追従
	NodeFollowing();

}

void PlayerHorn::ParentlessUpdate()
{
}

void PlayerHorn::ColliderInitialize()
{

	// OBB
	//OBB obb = std::get<OBB>(*collider_.get());
	//obb.SetParentObject(this);
	//ColliderShape* colliderShape = new ColliderShape();
	//*colliderShape = obb;
	//collider_.reset(colliderShape);

}

void PlayerHorn::ColliderUpdate()
{

	//OBB obb = std::get<OBB>(*collider_.get());

	//obb.center_ = worldTransform_.GetWorldPosition();

	//ColliderShape* colliderShape = new ColliderShape();

	//*colliderShape = obb;

	//collider_.reset(colliderShape);

}

void PlayerHorn::NodeFollowing()
{

	// 位置行列
	Vector3 pos = {
	parentNodeData_->matrix.m[3][0],
	parentNodeData_->matrix.m[3][1],
	parentNodeData_->matrix.m[3][2] };

	pos = Matrix4x4::Transform(pos + localPosition_, *parentMatrix_);

	// 回転行列
	Matrix4x4 rotateMatrix = Matrix4x4::Multiply(parentNodeData_->offsetMatrix, parentNodeData_->matrix);
	rotateMatrix.m[3][0] = 0.0f;
	rotateMatrix.m[3][1] = 0.0f;
	rotateMatrix.m[3][2] = 0.0f;

	Matrix4x4 parentRotateMatrix = *parentMatrix_;

	parentRotateMatrix.m[3][0] = 0.0f;
	parentRotateMatrix.m[3][1] = 0.0f;
	parentRotateMatrix.m[3][2] = 0.0f;

	rotateMatrix = Matrix4x4::Multiply(Matrix4x4::MakeRotateXYZMatrix(localRotate_), Matrix4x4::Multiply(rotateMatrix, parentRotateMatrix));

	Matrix4x4 scaleMatrix = Matrix4x4::MakeScaleMatrix(localScale_);

	worldTransform_.worldMatrix_ = Matrix4x4::Multiply(scaleMatrix, Matrix4x4::Multiply(rotateMatrix, Matrix4x4::MakeTranslateMatrix(pos)));

}
