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

private: // メンバ変数

	// 開始地点 (現在のプレイヤーの位置)
	Vector3 startPos_{};

	// 終了地点 (開始地点のy座標を下まで)
	Vector3 endPos_{};

	// 落下終了までの時間
	float endTime_ = 0.0f;

	// 経過時間
	float time_ = 0.0f;

};

