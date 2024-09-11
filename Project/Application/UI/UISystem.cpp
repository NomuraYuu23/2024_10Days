#include "UISystem.h"
#include "../../Engine/base/TextureManager.h"
#include "LeftStickUI.h"
#include "LeftStickBG.h"
#include "ButtonAUI.h"
#include "HPUI.h"
#include "RightStickBG.h"
#include "RightStickUI.h"

void UISystem::Initialize(DirectXCommon* dxCommon, Player* player)
{

	player_ = player;

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


}

void UISystem::Update()
{

	// UI
	for (uint32_t i = 0; i < kUIIndexOfCount; ++i) {
		UIs_[i]->Update();
	}

}

void UISystem::Draw()
{

	// UI
	for (uint32_t i = 0; i < kUIIndexOfCount; ++i) {
		UIs_[i]->Draw();
	}
	
}
