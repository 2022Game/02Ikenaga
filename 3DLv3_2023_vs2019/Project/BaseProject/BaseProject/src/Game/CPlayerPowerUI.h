#pragma once
#include "CUIBase.h"

class CText;

// 攻撃力のUI
class CPlayerPowerUI : public CUIBase
{
public:

	// コンストラク
	CPlayerPowerUI(float x, float y);

	// デストラクタ
	~CPlayerPowerUI();

	// 攻撃力を設定
	void SetPower(int power);

	// 描画
	void Render()override;
private:
	CText* mpPowerText;   // 攻撃力のテキスト
	CVector2 mBasePos;  // ベースの位置
};