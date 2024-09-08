#pragma once
#include "../../GPUParticle/CloudParticle/CloudParticle.h"

class Player;

class CloudSystem
{

public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="commandList">コマンドリスト</param>
	/// <param name="rootSignature">ルートシグネチャ</param>
	/// <param name="pipelineState">パイプラインステート</param>
	/// <param name="player"プレイヤー></param>
	void Initialize(
		ID3D12Device* device,
		ID3D12GraphicsCommandList* commandList,
		ID3D12RootSignature* rootSignature,
		ID3D12PipelineState* pipelineState,
		Player* player);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="color">色</param>
	void Update(const Vector4& color);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	/// <param name="camera">カメラ</param>
	void Draw(
		ID3D12GraphicsCommandList* commandList,
		BaseCamera& camera);

private:

	// パーティクル
	std::unique_ptr<CloudParticle> cloudParticle_;

	// プレイヤー
	Player* player_;

	// プレイヤーは下
	bool playerIsBelow_;

	// 雲の標準位置
	float cloudPosY_ = 30.0f;

	// プレイヤーのパワー範囲
	float pawerRadius_ = 0.0f;

	// パワー
	float power_ = 0.0f;

};

