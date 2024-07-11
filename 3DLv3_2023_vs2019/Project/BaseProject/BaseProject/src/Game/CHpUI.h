#pragma once
#include "CUIBase.h"

class CText;

// HPのUI
class CHpUI : public CUIBase
{
public:

	// コンストラク
	CHpUI();

	// デストラクタ
	~CHpUI();

	// Hpを設定
	void SetHpText(std::string hpText);

	// 描画
	void Render()override;
private:
	CText* mpHpText;    // Hpテキスト
	CVector2 mBasePos;  // ベースの位置
};