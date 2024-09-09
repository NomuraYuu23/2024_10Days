#include "EggBreak.CS.hlsli"
#include "../RandomGenerator/RandomGenerator.hlsli"

struct Emitter {
	float32_t3 translate; // 位置
	float32_t radius; // 射出半径
	uint32_t count; // 射出数
	float32_t frequency; // 射出間隔
	float32_t frequencyTime; // 射出間隔調整時間
	uint32_t emit; // 射出許可
};

ConstantBuffer<Emitter> gEmitter : register(b0);

ConstantBuffer<PerFrame> gPerFrame : register(b1);

RWStructuredBuffer<Particle> gParticles : register(u0);

RWStructuredBuffer<int32_t> gFreeListIndex : register(u1);

RWStructuredBuffer<uint32_t> gFreeList : register(u2);

[numthreads(1, 1, 1)]
void main(uint32_t3 DTid : SV_DispatchThreadID)
{

	if (gEmitter.emit != 0) {

		RandomGenerator generator;

		generator.seed = (DTid + gPerFrame.time) * gPerFrame.time;

		for (uint32_t conutIndex = 0; conutIndex < gEmitter.count; ++conutIndex) {

			int32_t freeListIndex;

			InterlockedAdd(gFreeListIndex[0], -1, freeListIndex);

			if (0 <= freeListIndex && freeListIndex < kMaxParticles) {

				int32_t particleIndex = gFreeList[freeListIndex];

				float32_t size = generator.Generate1d() + 0.1f;
				gParticles[particleIndex].scale = float32_t3(size, size, size);

				gParticles[particleIndex].translate = generator.Generate3d() * gEmitter.radius * 2.0f - gEmitter.radius + gEmitter.translate;

				gParticles[particleIndex].color.rgb = float32_t3(0.5f, 0.5f, 0.5f);
				gParticles[particleIndex].color.a = 1.0f;
				gParticles[particleIndex].lifeTime = generator.Generate1d() * 0.2f + 0.2f;

				float32_t velocityMax = 1.6f;

				gParticles[particleIndex].velocity =
					float32_t3(
						generator.Generate1d() * velocityMax - velocityMax * 0.5f,
						generator.Generate1d() * velocityMax * 0.5f,
						generator.Generate1d() * velocityMax - velocityMax * 0.5f);

				gParticles[particleIndex].velocity *= 0.5f;

				gParticles[particleIndex].currentTime = 0.0f;

				gParticles[particleIndex].rotate = float32_t3(0.0f, 0.0f, 0.0f);
				gParticles[particleIndex].rotateVelocity = float32_t3(0.0f, 0.0f, 0.0f);

				gParticles[particleIndex].rotateMatrix[0][0] = 1.0f;
				gParticles[particleIndex].rotateMatrix[0][1] = 0.0f;
				gParticles[particleIndex].rotateMatrix[0][2] = 0.0f;
				gParticles[particleIndex].rotateMatrix[0][3] = 0.0f;

				gParticles[particleIndex].rotateMatrix[1][0] = 0.0f;
				gParticles[particleIndex].rotateMatrix[1][1] = 1.0f;
				gParticles[particleIndex].rotateMatrix[1][2] = 0.0f;
				gParticles[particleIndex].rotateMatrix[1][3] = 0.0f;

				gParticles[particleIndex].rotateMatrix[2][0] = 0.0f;
				gParticles[particleIndex].rotateMatrix[2][1] = 0.0f;
				gParticles[particleIndex].rotateMatrix[2][2] = 1.0f;
				gParticles[particleIndex].rotateMatrix[2][3] = 0.0f;

				gParticles[particleIndex].rotateMatrix[3][0] = 0.0f;
				gParticles[particleIndex].rotateMatrix[3][1] = 0.0f;
				gParticles[particleIndex].rotateMatrix[3][2] = 0.0f;
				gParticles[particleIndex].rotateMatrix[3][3] = 1.0f;

			}
			else {

				InterlockedAdd(gFreeListIndex[0], 1);
				break;

			}

		}
	}

}