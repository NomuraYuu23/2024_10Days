#include "GameAudioManager.h"

GameAudioManager::~GameAudioManager()
{

}

void GameAudioManager::Initialize()
{

	AudioManager::Initialize();

	// 全体の初期設定
	for (uint32_t i = 0; i < GameAudioNameIndex::kGameAudioNameIndexOfCount; ++i) {
		audioDatas_[i].handle_ = audio_->LoadAudio(audioNames_[i]);
		audioDatas_[i].isLoop_ = false;
		audioDatas_[i].volume_ = 1.0f;
		audioDatas_[i].isBGM_ = false;
	}

	// 個別設定
	audioDatas_[kGamePteraSpawnSE].volume_ = 0.5f;
	audioDatas_[kGameEnemySpawnSE].volume_ = 0.5f;

	audioDatas_[kGamejumpSE].volume_ = 2.0f;
	audioDatas_[kGameEggBreakSE].volume_ = 1.5f;

	audioDatas_[kGameBossAttackSE].volume_ = 8.0f;
}

void GameAudioManager::Update()
{

}

void GameAudioManager::PlayWave(uint32_t audioIndex)
{

	float volume = 0.0f;
	if (audioDatas_[audioIndex].isBGM_) {
		volume = audioDatas_[audioIndex].volume_ * masterVolume_ * BGMVolume_;
	}
	else {
		volume = audioDatas_[audioIndex].volume_ * masterVolume_ * SEVolume_;
	}

	IXAudio2SourceVoice* pSourceVoice = audio_->PlayWave(audioDatas_[audioIndex].handle_, audioDatas_[audioIndex].isLoop_, volume);
	assert(pSourceVoice);
	for (uint32_t i = 0; i < kMaxPlayingSoundData; ++i) {
		if (playingSoundDatas_[i].handle_ == audioDatas_[audioIndex].handle_) {
			if (playingSoundDatas_[i].pSourceVoice_ != nullptr) {
				playingSoundDatas_[i].pSourceVoice_->DestroyVoice();
			}
			playingSoundDatas_[i].pSourceVoice_ = pSourceVoice;
			playingSoundDatas_[i].volume_ = volume;
			return;
		}
		if (playingSoundDatas_[i].handle_ == audio_->kMaxSoundData) {
			playingSoundDatas_[i].handle_ = audioDatas_[audioIndex].handle_;
			playingSoundDatas_[i].pSourceVoice_ = pSourceVoice;
			playingSoundDatas_[i].volume_ = volume;
			return;
		}
	}

}
