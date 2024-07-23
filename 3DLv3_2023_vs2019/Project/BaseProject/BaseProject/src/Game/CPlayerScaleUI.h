#pragma once
#include "CUIBase.h"

class CText;

// 大きさのUI
class CPlayerScaleUI : public CUIBase
{
public:

	// コンストラク
	CPlayerScaleUI(float x, float y);

	// デストラクタ
	~CPlayerScaleUI();

	// 大きさを設定
	void SetScale(float scale);

	// 描画
	void Render()override;
private:
	CText* mpScaleText;  // 攻撃力のテキスト
	CVector2 mBasePos;   // ベースの位置
};