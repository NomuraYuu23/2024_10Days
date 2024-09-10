#include "EggBreakParticleManager.h"

void EggBreakParticleManager::Initialize()
{

	dxCommon_ = DirectXCommon::GetInstance();

	// パーティクル
	eggBreakParticle_ = std::make_unique<EggBreakParticle>();
	eggBreakParticle_->Initialize(
		dxCommon_->GetDevice(),
		dxCommon_->GetCommadListLoad(),
		GraphicsPipelineState::sRootSignature[GraphicsPipelineState::kPipelineStateIndexGPUParticleBlendNormal].Get(),
		GraphicsPipelineState::sPipelineState[GraphicsPipelineState::kPipelineStateIndexGPUParticleBlendNormal].Get());

	// 発生位置
	emitPositions_ = {};

	// 現在の発生数
	emitNum_ = 0;

}

void EggBreakParticleManager::Update()
{

	EmitterCS emitter;
	emitter.count = 30;
	emitter.frequency = 1.0f;
	emitter.radius = 1.0f;

	// 生成しない
	if (emitNum_ == 0) {
		emitter.frequencyTime = 0.0f;
		emitter.translate = { 0.0f,0.0f,0.0f };
		emitter.emit = 0;
		eggBreakParticle_->SetEmitter(emitter, true);
	}
	else {
		emitter.frequencyTime = 1.0f;
		emitter.translate = emitPositions_[0];
		emitter.translate.y += 3.0f;
		emitter.emit = 1;

		eggBreakParticle_->SetEmitter(emitter, true);
		eggBreakParticle_->Update();
	}

}

void EggBreakParticleManager::Draw(BaseCamera& camera)
{

	eggBreakParticle_->Draw(dxCommon_->GetCommadList(), camera);

}

void EggBreakParticleManager::PositionRegister(const Vector3& position)
{

	if (emitNum_ == kEmitNumMax) {
		return;
	}

	emitPositions_[emitNum_] = position;

	emitNum_++;

}

void EggBreakParticleManager::PositionClear()
{

	for (uint32_t i = 0; i < kEmitNumMax; ++i) {
		emitPositions_[i] = { 0.0f,0.0f,0.0f };
	}

	emitNum_ = 0;

}
