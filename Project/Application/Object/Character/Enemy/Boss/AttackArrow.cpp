#include "AttackArrow.h"
#include "../../../Engine/Particle/BillBoardMatrix.h"
LevelData::MeshData AttackArrowObject::ArrowCreate()
{

	LevelData::MeshData data;
	static size_t id;
	// 名前
	data.name = "AttackArrow" + std::to_string(id++);
	// トランスフォーム
	data.transform = {
		3.0f,3.0f,1.0f,
		0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f
	};

	// ファイルの名前
	data.flieName = "attack_UI.obj";
	// ディレクトリパス
	data.directoryPath = "Resources/Model/Boss/";
	// クラスの名前
	data.className = "AttackArrowObject";
	// 親の名前
	data.parentName = "";

	// コライダー(一時的なもの、親部分はヌルにしとく)
	OBB obb;
	obb.Initialize({ 0.0f,0.0f,0.0f }, Matrix4x4::MakeIdentity4x4(), { 1.0f,2.5f,1.0f }, static_cast<Null*>(nullptr));
	data.collider = obb;

	return data;

}

void AttackArrowObject::Update(const Vector3& pos)
{

	MeshObject::Update();

	worldTransform_.transform_.translate = pos;

	theta_ += 0.1f;
	if (theta_> 3.141592f*2.0f) {
		theta_ = 0;
	}
	worldTransform_.transform_.translate.y += offset_ + std::sinf(theta_) * moveLength_;


}

void AttackArrowObject::Draw(BaseCamera& camera)
{


	worldTransform_.UpdateMatrix(BillBoardMatrix::GetBillBoardMatrixY(camera));
	MeshObject::Draw(camera);
}