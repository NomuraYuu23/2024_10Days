#include "UISystem.h"
#include "../../Engine/base/TextureManager.h"
#include "LeftStickUI.h"
#include "LeftStickBG.h"
#include "ButtonAUI.h"
#include "HPUI.h"
#include "RightStickBG.h"
#include "RightStickUI.h"
#include "HPNumUI.h"
#include "TimeTenNumUI.h"
#include "TimeOneNumUI.h"

void UISystem::Initialize(DirectXCommon* dxCommon, Player* player, uint32_t* dayCount)
{

	player_ = player;

	// タイトルの追加位置
	titlePosAddX_ = -640.0f;

	tutorialPosAddX_ = -640.0f;

	dayCount_ = dayCount;

	// テクスチャハンドル
	for (uint32_t i = 0; i < kUITextureHandleIndexOfCount; ++i) {
		textureHandles_[i] = TextureManager::Load(textureNames_[i], dxCommon);
	}

	// UIはそれぞれ個別で初期化

	// 左スティック
	UIs_[kUIIndexLeftStick] = std::make_unique<LeftStickUI>();
	UIs_[kUIIndexLeftStick]->Initialize(textureHandles_[kUITextureHandleIndexLeftStick], "LeftStick");

	// 左スティック背景
	UIs_[kUIIndexLeftStickBG] = std::make_unique<LeftStickBG>();
	UIs_[kUIIndexLeftStickBG]->Initialize(textureHandles_[kUITextureHandleIndexStickBG], "LeftStick");
	static_cast<LeftStickBG*>(UIs_[kUIIndexLeftStickBG].get())->SetParent(static_cast<LeftStickUI*>(UIs_[kUIIndexLeftStick].get()));

	// 左スティック文字
	UIs_[kUIIndexLeftStickStr] = std::make_unique<UI>();
	UIs_[kUIIndexLeftStickStr]->Initialize(textureHandles_[kUITextureHandleIndexLeftStickStr], "LeftStickString");

	// Aボタン
	UIs_[kUIIndexButtonA] = std::make_unique<ButtonAUI>();
	UIs_[kUIIndexButtonA]->Initialize(textureHandles_[kUITextureHandleIndexButtonA], "ButtonA");

	// Aボタン文字
	UIs_[kUIIndexButtonAStr] = std::make_unique<UI>();
	UIs_[kUIIndexButtonAStr]->Initialize(textureHandles_[kUITextureHandleIndexButtonAStr], "ButtonAString");


	// HP1
	UIs_[kUIIndexHP1] = std::make_unique<HPUI>();
	static_cast<HPUI*>(UIs_[kUIIndexHP1].get())->Initialize(player_, 2, textureHandles_[kUITextureHandleIndexHPHorn], "HP");

	// HP0
	UIs_[kUIIndexHP0] = std::make_unique<HPUI>();
	static_cast<HPUI*>(UIs_[kUIIndexHP0].get())->Initialize(player_, 1, textureHandles_[kUITextureHandleIndexHPHead], "HP");

	// HP2
	UIs_[kUIIndexHP2] = std::make_unique<HPUI>();
	static_cast<HPUI*>(UIs_[kUIIndexHP2].get())->Initialize(player_, 3, textureHandles_[kUITextureHandleIndexHPHorn], "HP");


	// 右スティック
	UIs_[kUIIndexRightStick] = std::make_unique<RightStickUI>();
	UIs_[kUIIndexRightStick]->Initialize(textureHandles_[kUITextureHandleIndexRightStick], "RightStick");

	// 右スティック背景
	UIs_[kUIIndexRightStickBG] = std::make_unique<RightStickBG>();
	UIs_[kUIIndexRightStickBG]->Initialize(textureHandles_[kUITextureHandleIndexStickBG], "RightStick");
	static_cast<RightStickBG*>(UIs_[kUIIndexRightStickBG].get())->SetParent(static_cast<RightStickUI*>(UIs_[kUIIndexRightStick].get()));

	// 右スティック文字
	UIs_[kUIIndexRightStickStr] = std::make_unique<UI>();
	UIs_[kUIIndexRightStickStr]->Initialize(textureHandles_[kUITextureHandleIndexRightStickStr], "RightStickString");

	//  HP文字
	UIs_[kUIIndexHPStr] = std::make_unique<UI>();
	UIs_[kUIIndexHPStr]->Initialize(textureHandles_[kUITextureHandleIndexHPStr], "HPString");

	
	//  HP数字
	UIs_[kUIIndexHPNum] = std::make_unique<HPNumUI>();
	std::array<uint32_t, 4> textureHandles;
	textureHandles[0] = textureHandles_[kUITextureHandleIndex0];
	textureHandles[1] = textureHandles_[kUITextureHandleIndex1];
	textureHandles[2] = textureHandles_[kUITextureHandleIndex2];
	textureHandles[3] = textureHandles_[kUITextureHandleIndex3];
	static_cast<HPNumUI*>(UIs_[kUIIndexHPNum].get())->Initialize(player_, textureHandles, "HPNum");

	//  経過時間文字
	UIs_[kUIIndexTimeStr] = std::make_unique<UI>();
	UIs_[kUIIndexTimeStr]->Initialize(textureHandles_[kUITextureHandleIndexTimeStr], "TimeString");

	std::array<uint32_t, 10> timeTextureHandles;
	timeTextureHandles[0] = textureHandles_[kUITextureHandleIndex0];
	timeTextureHandles[1] = textureHandles_[kUITextureHandleIndex1];
	timeTextureHandles[2] = textureHandles_[kUITextureHandleIndex2];
	timeTextureHandles[3] = textureHandles_[kUITextureHandleIndex3];
	timeTextureHandles[4] = textureHandles_[kUITextureHandleIndex4];
	timeTextureHandles[5] = textureHandles_[kUITextureHandleIndex5];
	timeTextureHandles[6] = textureHandles_[kUITextureHandleIndex6];
	timeTextureHandles[7] = textureHandles_[kUITextureHandleIndex7];
	timeTextureHandles[8] = textureHandles_[kUITextureHandleIndex8];
	timeTextureHandles[9] = textureHandles_[kUITextureHandleIndex9];

	UIs_[kUIIndexTimeTenNum] = std::make_unique<TimeTenNumUI>();
	static_cast<TimeTenNumUI*>(UIs_[kUIIndexTimeTenNum].get())->Initialize(dayCount_, timeTextureHandles, "TimeTenNum");
	UIs_[kUIIndexTimeOneNum] = std::make_unique<TimeOneNumUI>();
	static_cast<TimeOneNumUI*>(UIs_[kUIIndexTimeOneNum].get())->Initialize(dayCount_, timeTextureHandles, "TimeOneNum");

}

void UISystem::Update()
{

	// UI
	Vector2 pos = { 0.0f,0.0f };
	for (uint32_t i = 0; i < kUIIndexOfCount; ++i) {
		UIs_[i]->Update();
		if (titlePosAddX_ != 0.0f) {
			pos = UIs_[i]->GetPosition();
			pos.x += titlePosAddX_;
			UIs_[i]->SetPosition(pos);
		}
		if (tutorialPosAddX_ != 0.0f) {
			if (i >= kUIIndexHP1) {
				pos = UIs_[i]->GetPosition();
				pos.x += tutorialPosAddX_;
				UIs_[i]->SetPosition(pos);
			}
		}
	}

}

void UISystem::Draw()
{

	// UI
	for (uint32_t i = 0; i < kUIIndexOfCount; ++i) {
		UIs_[i]->Draw();
	}
	
}
