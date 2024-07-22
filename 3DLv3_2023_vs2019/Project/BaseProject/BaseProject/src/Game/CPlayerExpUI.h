#pragma once
#include "CUIBase.h"

class CText;

// プレイヤーExpのUI
class CPlayerExpUI : public CUIBase
{
public:

	// コンストラク
	CPlayerExpUI(float x, float y);

	// デストラクタ
	~CPlayerExpUI();

	// プレイヤーEXPを設定
	void SetExp(int exp);

	// 描画
	void Render()override;
private:
	CText* mpExpText;
	CVector2 mBasePos;
};
