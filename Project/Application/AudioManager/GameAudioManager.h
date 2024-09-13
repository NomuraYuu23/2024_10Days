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
	kGameEnemySpawnSE, // スピノスポーンSE
	kGameEnemyBulletSE, // スピノバレットSE
	kGameBossAttackSE, // ボス攻撃SE
	kGameBossRoarSE, // ボス咆哮SE
	kGameGameStartSE, // スタートSE
	kGameHitBlockSE, // ヒットブロックSE
	kGameHitAttackSE, // ヒットアタックSE
	kGameClearBGM, // クリアBGM
	kGameBossFangSE, // ボス噛みつきSE
	kGameBossHitSE, // ボス被弾SE
	kGameBossBGM, // ボスBGM

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
		"Sound/BGM/BGM.mp3",
		"Sound/SE/jump.mp3",
		"Sound/SE/block.mp3",
		"Sound/SE/Ptera_Spawn.mp3",
		"Sound/SE/Move.mp3",
		"Sound/SE/HeadDrop.mp3",
		"Sound/SE/Egg_Break.mp3",
		"Sound/SE/Enemy_Spawn.mp3",
		"Sound/SE/Enemy_Bullet.mp3",
		"Sound/SE/Boss_Attack_2.mp3",
		"Sound/SE/Boss_Roar.mp3",
		"Sound/SE/gameStart.mp3",
		"Sound/SE/Hit_Block.mp3",
		"Sound/SE/Hit_Attack.mp3",
		"Sound/BGM/gameClear.mp3",
		"Sound/SE/Boss_Fang.mp3",
		"Sound/SE/Boss_Hit.mp3",
		"Sound/BGM/boss_BGM.mp3",
	};

};

