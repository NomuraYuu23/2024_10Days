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

	EggBreakParticle::EmitBlendNormalCS emitter;
	emitter.count = 30;
	emitter.frequency = 1.0f;
	emitter.radius = 1.0f;
	emitter.num = emitNum_;

	// 生成しない
	if (emitNum_ == 0) {
		emitter.frequencyTime = 0.0f;
		emitter.translate0 = { 0.0f,0.0f,0.0f };
		emitter.translate1 = { 0.0f,0.0f,0.0f };
		emitter.translate2 = { 0.0f,0.0f,0.0f };
		emitter.translate3 = { 0.0f,0.0f,0.0f };

		emitter.emit = 0;
		eggBreakParticle_->SetEmitter(emitter, true);
	}
	else {
		emitter.frequencyTime = 1.0f;
		emitter.translate0 = emitPositions_[0];
		emitter.translate1 = emitPositions_[1];
		emitter.translate2 = emitPositions_[2];
		emitter.translate3 = emitPositions_[3];
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
	// 追加分
	emitPositions_[emitNum_].y += 3.0f;

	emitNum_++;

}

void EggBreakParticleManager::PositionClear()
{

	for (uint32_t i = 0; i < kEmitNumMax; ++i) {
		emitPositions_[i] = { 0.0f,0.0f,0.0f };
	}

	emitNum_ = 0;

}
