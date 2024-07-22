#pragma once
#include "CUIBase.h"

class CText;

// プレイヤーSPのUI
class CPlayerSpUI : public CUIBase
{
public:

	// コンストラク
	CPlayerSpUI(float x, float y);

	// デストラクタ
	~CPlayerSpUI();

	// プレイヤーSPを設定
	void SetSp(int sp);

	// 描画
	void Render()override;
private:
	CText* mpSpText;
	CVector2 mBasePos;
};