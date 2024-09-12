#pragma once
#include "../../../Engine/Object/MeshObject.h"
class TutorialArrowObject :
    public MeshObject
{

public: // 関数

    /// <summary>
    /// 更新処理
    /// </summary>
    void Update(const Matrix4x4& roatateMatrix);

};

