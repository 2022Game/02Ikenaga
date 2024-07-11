#pragma once
#include "CUIBase.h"

class CText;

// LvのUI
class CLevelUI : public CUIBase
{
public:

	// コンストラク
	CLevelUI();

	// デストラクタ
	~CLevelUI();

	// Levelを設定
	void SetLevel(std::string level);

	// 更新
	void Update()override;

	// 描画
	void Render()override;
private:
	CText* mpLevelText;
	CVector2 mBasePos;
};