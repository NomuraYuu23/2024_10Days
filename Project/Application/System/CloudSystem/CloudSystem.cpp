#include "CloudSystem.h"

void CloudSystem::Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, ID3D12RootSignature* rootSignature, ID3D12PipelineState* pipelineState)
{

	cloudParticle = std::make_unique<CloudParticle>();
	cloudParticle->Initialize(device, commandList, rootSignature, pipelineState);

}

void CloudSystem::Update()
{

	cloudParticle->Update();

}

void CloudSystem::Draw(ID3D12GraphicsCommandList* commandList, BaseCamera& camera)
{

	cloudParticle->Draw(commandList, camera);

}
