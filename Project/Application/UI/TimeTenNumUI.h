#pragma once
#include "UI.h"
class TimeTenNumUI :
    public UI
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>	
	/// <param name="dayCount"></param>
	/// <param name="textureHandles"></param>
	/// <param name="groupName">グループ名</param>
	/// <param name="jsonName">json名</param>
	virtual void Initialize(uint32_t* dayCount,
		const std::array<uint32_t, 10>& textureHandles,
		const std::string& groupName, const std::string& jsonName = "UI");

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

private:

	uint32_t* dayCount_;

	std::array<uint32_t, 10> textureHandles_;

};

