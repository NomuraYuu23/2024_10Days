#include "UISystem.h"
#include "../../Engine/base/TextureManager.h"

void UISystem::Initialize(DirectXCommon* dxCommon)
{

	// テクスチャハンドル
	for (uint32_t i = 0; i < kUIIndexOfCount; ++i) {
		textureHandles_[i] = TextureManager::Load(textureNames_[i], dxCommon);
	}

	// UIはそれぞれ個別で初期化

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
