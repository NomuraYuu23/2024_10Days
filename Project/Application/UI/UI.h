#pragma once
#include <memory>
#include <cstdint>
#include "../../Engine/2D/Sprite.h"

class UI
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <param name="groupName">グループ名</param>
	/// <param name="jsonName">json名</param>
	virtual void Initialize(uint32_t textureHandle, const std::string& groupName, const std::string& jsonName = "UI");

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ関数

	/// <summary>
	/// 外部変数登録
	/// </summary>
	virtual void RegisteringGlobalVariables();

	/// <summary>
	/// 外部変数適用
	/// </summary>
	virtual void ApplyGlobalVariables();

protected: // メンバ変数

	//スプライト
	std::unique_ptr<Sprite> sprite_;

	// ポジション
	Vector2 position_;

	// 回転
	float rotate_;

	//サイズ
	Vector2 size_;

	// json名
	std::string jsonName_;

	// グループ名
	std::string groupName_;

public:

	/// <summary>
	/// セットカラー
	/// </summary>
	/// <param name="color"></param>
	void SetColor(const Vector4& color);

	/// <summary>
	/// 表示するか
	/// </summary>
	/// <param name="isInvisible"></param>
	void SetIsInvisible(bool isInvisible);

	/// <summary>
	/// 表示するか
	/// </summary>
	bool GetIsInvisible();

	/// <summary>
	/// テクスチャハンドル
	/// </summary>
	/// <param name="textureHandle"></param>
	void SetTextureHandle(uint32_t textureHandle);

	/// <summary>
	/// 回転
	/// </summary>
	/// <param name="rotate"></param>
	void SetRotate(float rotate);

	/// <summary>
	/// ポジションアクセッサ
	/// </summary>
	/// <param name="positon"></param>
	void SetPosition(const Vector2& positon);
	Vector2 GetPosition();

	/// <summary>
	/// サイズアクセッサ
	/// </summary>
	/// <param name="size"></param>
	void SetSize(const Vector2& size);
	Vector2 GetSize();

	/// <summary>
	/// アンカーポイント
	/// </summary>
	/// <param name="anchorPoint"></param>
	void SetAnchorPoint(const Vector2& anchorPoint);

};