#pragma once
#include "CUIBase.h"

class CText;

// プレイヤーHPのUI
class CPlayerHpUI : public CUIBase
{
public:

	// コンストラク
	CPlayerHpUI(float x, float y);

	// デストラクタ
	~CPlayerHpUI();

	// プレイヤーHPを設定
	void SetHp(int hp);

	// 描画
	void Render()override;
private:
	CText* mpHpText;
	CVector2 mBasePos;
};
