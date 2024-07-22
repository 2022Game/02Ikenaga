#pragma once
#include "CUIBase.h"

class CText;

// プレイヤー最大HPのUI
class CPlayerMaxHpUI : public CUIBase
{
public:

	// コンストラク
	CPlayerMaxHpUI(float x, float y);

	// デストラクタ
	~CPlayerMaxHpUI();

	// プレイヤー最大HPを設定
	void SetMaxHp(int hp);

	// 描画
	void Render()override;
private:
	CText* mpMaxHpText;
	CVector2 mBasePos;
};