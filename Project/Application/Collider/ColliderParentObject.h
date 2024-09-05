#pragma once
#include <variant>
#include "../../Engine/Collision/CollisionData.h"

class Null;
class BaseObstacle;
class BaseRigidBodyObject;
class Player;

using ColliderParentObject = std::variant<Null*, BaseObstacle*, BaseRigidBodyObject*, Player*>;

// 親がないもの用
class Null {

public:

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="colliderPartner"></param>
	/// <param name="collisionData"></param>
	void OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData) {}

};
