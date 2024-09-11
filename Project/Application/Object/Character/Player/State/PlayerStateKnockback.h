#pragma once
#include "IPlayerState.h"
#include "../../../../../Engine/3D/WorldTransform.h"

class PlayerStateKnockback :
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

	// 初めの速さ
	float initSpeed_;

	// 速度
	Vector3 velocity_;

	// 加速度
	Vector3 acceleration_;

	// 終了フレーム
	uint32_t endFrame_;

	// 現在のフレーム
	uint32_t countFrame_;

	// 上昇速度
	float climbSpeed_;

};

