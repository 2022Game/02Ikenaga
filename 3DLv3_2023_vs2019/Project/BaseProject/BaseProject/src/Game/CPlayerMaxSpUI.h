#pragma once
#include "CUIBase.h"

class CText;

// プレイヤーの最大SPのUI
class CPlayerMaxSpUI : public CUIBase
{
public:

	// コンストラク
	CPlayerMaxSpUI(float x, float y);

	// デストラクタ
	~CPlayerMaxSpUI();

	// プレイヤーの最大SPを設定
	void SetMaxSp(int maxSp);

	// 描画
	void Render()override;
private:
	CText* mpMaxSpText;
	CVector2 mBasePos;
};