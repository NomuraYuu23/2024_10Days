#pragma once
#include "../../Engine/Audio/AudioManager.h"

enum GameAudioNameIndex {
	kGameBGM, // BGM
	kGamejumpSE, // ジャンプSE
	kGameBlockSE, // ブロックSE
	kGamePteraSpawnSE, // プテラスポーンSE
	kGameMoveSE, // 移動SE
	kGameHeadDropSE, // ヘッドドロップSE
	kGameEggBreakSE, // 卵破壊SE
	kGameAudioNameIndexOfCount // 数
};

class GameAudioManager : public AudioManager
{

public: // メンバ関数

	~GameAudioManager();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 音声再生
	/// </summary>
	/// <param name="audioIndex">オーディオの名前</param>
	/// <returns>再生中のサウンドデータの番号</returns>
	void PlayWave(uint32_t audioIndex) override;

private: // メンバ変数

	// データ
	std::array<AudioData, GameAudioNameIndex::kGameAudioNameIndexOfCount> audioDatas_;

	// ファイル名
	std::array<const std::string, GameAudioNameIndex::kGameAudioNameIndexOfCount> audioNames_ = 
	{
		"Sound/BGM/inGameBGM.mp3",
		"Sound/SE/jump.mp3",
		"Sound/SE/block.mp3",
		"Sound/SE/Ptera_Spawn.mp3",
		"Sound/SE/Move.mp3",
		"Sound/SE/HeadDrop.mp3",
		"Sound/SE/Egg_Break.mp3",
		
	};

};

