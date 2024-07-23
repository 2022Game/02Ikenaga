#pragma once
#include "CUIBase.h"

class CText;

// 防御力のUI
class CPlayerDefenseUI : public CUIBase
{
public:

	// コンストラク
	CPlayerDefenseUI(float x, float y);

	// デストラクタ
	~CPlayerDefenseUI();

	// 防御力を設定
	void SetDefense(int defense);

	// 描画
	void Render()override;
private:
	CText* mpDefenseText;  // 防御力のテキスト
	CVector2 mBasePos;     // ベースの位置
};