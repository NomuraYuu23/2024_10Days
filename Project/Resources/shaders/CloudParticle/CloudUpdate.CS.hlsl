#include "Cloud.CS.hlsli"
#include "../RandomGenerator/RandomGenerator.hlsli"

struct Power{

	float32_t3 position_;
	float32_t power_;
	float32_t radius_;
	uint32_t execution_;

};

RWStructuredBuffer<Particle> gParticles : register(u0);

ConstantBuffer<PerFrame> gPerFrame : register(b0);

RWStructuredBuffer<int32_t> gFreeListIndex : register(u1);

RWStructuredBuffer<uint32_t> gFreeList : register(u2);

ConstantBuffer<Power> gPower : register(b1);

[numthreads(1024, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{

	uint32_t particleIndex = DTid.x;
	if (particleIndex < kMaxParticles) {

		if (gParticles[particleIndex].color.a != 0) {

			if (gPower.execution_ == 1) {
				float32_t3 powerPos = gPower.position_;
				powerPos.y = gParticles[particleIndex].translate.y;
				if (distance(gParticles[particleIndex].translate, gPower.position_) <= gPower.radius_) {
					float32_t3 norm = normalize(gParticles[particleIndex].translate - gPower.position_);
					gParticles[particleIndex].velocity = norm * gPower.power_;
				}

			}

			gParticles[particleIndex].translate += gParticles[particleIndex].velocity;

			gParticles[particleIndex].currentTime += gPerFrame.deltaTime;

			float32_t t = gParticles[particleIndex].currentTime * rcp(gParticles[particleIndex].lifeTime);

			float32_t alpha = 0.0f;

			if (t <= 0.5f) {
				t *= 2.0f;
				alpha = t;
			}
			else {
				t -= 0.5f;
				t *= 2.0f;
				alpha = 1.0f - t;
				if (alpha < 0.0f) {
					alpha = 0.0f;
				}
			}
			gParticles[particleIndex].color.a = saturate(alpha);
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