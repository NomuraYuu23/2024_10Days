#pragma once
#include <variant>
#include "../../Engine/Collision/CollisionData.h"

class Null;
class BaseObstacle;
class BaseRigidBodyObject;
class Player;
class ShockWave;
class Enemy;
class Bullet;

using ColliderParentObject = std::variant<Null*, BaseObstacle*, BaseRigidBodyObject*, Player*,ShockWave*,Enemy*,Bullet*>;

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
