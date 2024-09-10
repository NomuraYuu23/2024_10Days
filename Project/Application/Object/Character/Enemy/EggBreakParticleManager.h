#pragma once
#include "../../../GPUParticle/EggBreakParticle/EggBreakParticle.h"
class EggBreakParticleManager
{

public: 

	void Initialize();

	void Update();

	void Draw();

private:

	// パーティクル
	std::unique_ptr<EggBreakParticle> eggBreakParticle_;

	// パーティクルフラグ
	bool emit_ = false;

};

