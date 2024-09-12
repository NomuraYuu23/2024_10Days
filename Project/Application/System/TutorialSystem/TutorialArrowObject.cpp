#include "TutorialArrowObject.h"

void TutorialArrowObject::Update(const Matrix4x4& roatateMatrix)
{
	
	MeshObject::Update();

	worldTransform_.UpdateMatrix(roatateMatrix);

}
