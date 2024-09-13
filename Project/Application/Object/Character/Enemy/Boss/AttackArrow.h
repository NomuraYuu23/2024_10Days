#pragma once
#include "../../../Engine/Object/MeshObject.h"
class AttackArrowObject :
    public MeshObject
{
public:
    static LevelData::MeshData ArrowCreate();

public: // 関数

    /// <summary>
    /// 更新処理
    /// </summary>
    void Update(const Vector3& pos);

    void Draw(BaseCamera& camera);
private :
    float offset_ = 5.0f;
    float theta_=0;
    float moveLength_ = 1.0f;
};

