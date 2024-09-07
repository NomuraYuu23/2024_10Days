#pragma once
#include "IPlayerState.h"
#include "../../../../../Engine/3D/WorldTransform.h"
class PlayerStateHeadDrop :
    public IPlayerState
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

private:

	/// <summary>
	/// 移動
	/// </summary>
	void Move(Vector3& move, WorldTransform* worldTransform, float speed);

private: // メンバ変数

};

