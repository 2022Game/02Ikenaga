#pragma once
#include "CUIBase.h"

class CText;

// プレイヤー最大EXPのUI
class CPlayerMaxExpUI : public CUIBase
{
public:

	// コンストラク
	CPlayerMaxExpUI(float x, float y);

	// デストラクタ
	~CPlayerMaxExpUI();

	// プレイヤー最大EXPを設定
	void SetMaxExp(int maxExp);

	// 描画
	void Render()override;
private:
	CText* mpMaxExpText;
	CVector2 mBasePos;
};