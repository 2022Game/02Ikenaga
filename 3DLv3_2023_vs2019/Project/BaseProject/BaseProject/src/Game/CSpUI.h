#pragma once
#include "CUIBase.h"

class CText;

// SPのUI
class CSpUI : public CUIBase
{
public:

	// コンストラク
	CSpUI();

	// デストラクタ
	~CSpUI();

	// Spを設定
	void SetSpText(std::string spText);

	// 描画
	void Render()override;
private:
	CText* mpSpText;    // Spテキスト
	CVector2 mBasePos;  // ベース位置
};