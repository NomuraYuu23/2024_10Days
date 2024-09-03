#include "ColliderParentIdentification.h"

#include "../Object/Obstacle/BaseObstacle.h"
#include "../Object/RigidBodyObject/BaseRigidBodyObject.h"

MeshObject* ColliderParentIdentification::Execution(ColliderParentObject colliderPartner)
{

    MeshObject* result = nullptr;

    if(std::holds_alternative<BaseObstacle*>(colliderPartner)){
        result = std::get<BaseObstacle*>(colliderPartner);
    }
    else if (std::holds_alternative<BaseRigidBodyObject*>(colliderPartner)) {
        result = std::get<BaseRigidBodyObject*>(colliderPartner);
    }

    return result;

}
