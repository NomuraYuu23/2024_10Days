#include "PostEffect.hlsli"

#include "PostEffectParameters.hlsli"

#include "PostEffectCalc.CS.hlsl"

ConstantBuffer<ComputeParameters> gComputeConstants : register(b0);

Texture2D<float32_t4> sourceImage0 : register(t0);

RWTexture2D<float32_t4> destinationImage0 : register(u0);

RWTexture2D<float32_t4> destinationImage1 : register(u1);

float32_t4 DOROKERA2(in const float32_t2 index) {

	// 入力色
	float32_t4 input = { 0.0f,0.0f,0.0f,0.0f };

	// 出力色
	float32_t4 output = { 0.0f,0.0f,0.0f,0.0f };

	// 一時的なインデックス
	float32_t2 indexTmp = { 0.0f,0.0f };

	// 重み
	float32_t weight = 0.0f;

	// 重み合計
	float32_t weightSum = 0.0f;

	// ベース色
	float32_t3 baseColor = { 0.49f, 0.78f, 0.58f };
	float32_t3 baseHSV = RGBToHSV(baseColor);

	for (int32_t i = -gComputeConstants.kernelSize * rcp(2); i < gComputeConstants.kernelSize * rcp(2); i += 2) {

		// インデックス
		indexTmp = index;
		indexTmp.y += float32_t(i) + 0.5f;

		if ((indexTmp.y < 0.0f) || (indexTmp.y >= float32_t(gComputeConstants.threadIdTotalY))) {
			continue;
		}

		input = destinationImage1[indexTmp];

		// 重み確認
		weight = Gauss(float32_t(i), gComputeConstants.gaussianSigma) + Gauss(float32_t(i) + 1.0f, gComputeConstants.gaussianSigma);

		// 色確認
		float32_t3 hsv = RGBToHSV(input.rgb);
		float32_t colorDifference = length(hsv.x - baseHSV.x);
		if (colorDifference < gComputeConstants.threshold) {
			// outputに加算
			output += input * weight;
		}
		// 重みの合計に加算
		weightSum += weight;
	}

	// 重みの合計分割る
	output *= rcp(weightSum);

	if (gComputeConstants.executionFlag & 1) {
		float32_t2 texcoord = GetTexcoord(index, float32_t2(gComputeConstants.threadIdTotalX, gComputeConstants.threadIdTotalY));

		float32_t2 corrent = texcoord * (1.0f - texcoord.yx);

		float32_t vigntte = corrent.x * corrent.y * gComputeConstants.vignetteSize;

		vigntte = saturate(pow(vigntte, gComputeConstants.vignetteChange));

		output.gb *= vigntte;
	}

	return output;

}

[numthreads(THREAD_X, THREAD_Y, THREAD_Z)]
void main(uint32_t3 dispatchId : SV_DispatchThreadID) {

	if (dispatchId.x < gComputeConstants.threadIdTotalX &&
		dispatchId.y < gComputeConstants.threadIdTotalY) {

		float32_t4 input0 = sourceImage0[dispatchId.xy];
		float32_t4 input1 = DOROKERA2(dispatchId.xy);
		destinationImage0[dispatchId.xy] = BlurAdd(input0, input1);

	}

}