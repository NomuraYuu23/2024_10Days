#include "EggBreak.CS.hlsli"
#include "../RandomGenerator/RandomGenerator.hlsli"

RWStructuredBuffer<Particle> gParticles : register(u0);

ConstantBuffer<PerFrame> gPerFrame : register(b0);

RWStructuredBuffer<int32_t> gFreeListIndex : register(u1);

RWStructuredBuffer<uint32_t> gFreeList : register(u2);

[numthreads(1024, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{

	uint32_t particleIndex = DTid.x;
	if (particleIndex < kMaxParticles) {

		if (gParticles[particleIndex].color.a != 0) {

			float32_t3 velocity = gParticles[particleIndex].velocity;
			float32_t3 acceleration = float32_t3(0.0f, 0.0f, 0.0f);

			acceleration.y = -0.016f;

			velocity += acceleration;
			gParticles[particleIndex].velocity = velocity;

			gParticles[particleIndex].translate += velocity;
			
			gParticles[particleIndex].currentTime += gPerFrame.deltaTime;

			if (1.0f <= gParticles[particleIndex].currentTime * rcp(gParticles[particleIndex].lifeTime)) {
				gParticles[particleIndex].color.a = 0.0f;
			}
		}
		else {

			gParticles[particleIndex].scale = float32_t3(0.0f, 0.0f, 0.0f);
			int32_t freeListIndex;
			InterlockedAdd(gFreeListIndex[0], 1, freeListIndex);

			if ((freeListIndex + 1) < kMaxParticles) {
				gFreeList[freeListIndex + 1] = particleIndex;
			}
			else {
				InterlockedAdd(gFreeListIndex[0], -1, freeListIndex);
			}

		}

	}

}