#include "CloudSystem.h"
#include "../../Object/Character/Player/Player.h"

void CloudSystem::Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, ID3D12RootSignature* rootSignature, ID3D12PipelineState* pipelineState, Player* player)
{

	cloudParticle_ = std::make_unique<CloudParticle>();
	cloudParticle_->Initialize(device, commandList, rootSignature, pipelineState);

	cloudPosY_ = 33.0f;

	EmitterCS emitter;
	emitter.count = 128;
	emitter.frequency = 0.2f;
	emitter.frequencyTime = 0.2f;
	emitter.translate = Vector3(0.0f, cloudPosY_, 0.0f);
	emitter.radius = 18.0f;
	emitter.emit = 0;

	cloudParticle_->SetEmitter(emitter, true);

	player_ = player;

	playerIsBelow_ = true;

	pawerRadius_ = 10.0f;

	power_ = 0.5f;

}

void CloudSystem::Update(const Vector4& color)
{

	cloudParticle_->Update();

	// 雲に力を加えるか
	Vector3 playerPos = player_->GetWorldTransformAdress()->GetWorldPosition();

	CloudParticle::Power power;
	power.position_ = playerPos;
	power.power_ = power_;
	power.radius_ = pawerRadius_;
	power.execution_ = 0;

	// プレイヤーは下にいた
	if (playerIsBelow_) {
		if (cloudPosY_ <= playerPos.y) {
			playerIsBelow_ = false;
			power.execution_ = 1;
		}
	}
	else {
		if (cloudPosY_ >= playerPos.y) {
			playerIsBelow_ = true;
			power.execution_ = 1;
		}
	}

	cloudParticle_->SetPowerMap(power);

	Vector4 materialColor = { 0.1f,0.1f,0.1f,1.0f };
	materialColor.x += color.x;
	materialColor.y += color.y;
	materialColor.z += color.z;

	materialColor.x /= 2.0f;
	materialColor.y /= 2.0f;
	materialColor.z /= 2.0f;

	cloudParticle_->GetMaterial()->SetColor(materialColor);

}

void CloudSystem::Draw(ID3D12GraphicsCommandList* commandList, BaseCamera& camera)
{

	cloudParticle_->Draw(commandList, camera);

}
