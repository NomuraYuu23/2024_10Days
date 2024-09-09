#include "UISystem.h"
#include "../../Engine/base/TextureManager.h"
#include "LeftStickUI.h"
#include "LeftStickBG.h"

void UISystem::Initialize(DirectXCommon* dxCommon)
{

	// テクスチャハンドル
	for (uint32_t i = 0; i < kUIIndexOfCount; ++i) {
		textureHandles_[i] = TextureManager::Load(textureNames_[i], dxCommon);
	}

	// UIはそれぞれ個別で初期化

	// 左スティック
	UIs_[kUIIndexLeftStick] = std::make_unique<LeftStickUI>();
	UIs_[kUIIndexLeftStick]->Initialize(textureHandles_[kUITextureHandleIndexLeftStick], "LeftStick");

	// 左スティック背景
	UIs_[kUIIndexLeftStickBG] = std::make_unique<LeftStickBG>();
	UIs_[kUIIndexLeftStickBG]->Initialize(textureHandles_[kUITextureHandleIndexLeftStickBG], "LeftStick");
	static_cast<LeftStickBG*>(UIs_[kUIIndexLeftStickBG].get())->SetParent(static_cast<LeftStickUI*>(UIs_[kUIIndexLeftStick].get()));

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
