#pragma once
#include "IPlayerState.h"
#include "../../../../../Engine/3D/WorldTransform.h"

class PlayerStateFloating :
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

	// 目指す向き
	Vector3 targetDirection_;

	// 目指す向きへの補間係数
	float targetAngleT_;

};

