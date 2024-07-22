#pragma once
#include "CUIBase.h"

class CText;

// プレイヤーレベルのUI
class CPlayerLevelUI : public CUIBase
{
public:

	// コンストラク
	CPlayerLevelUI(float x,float y);

	// デストラクタ
	~CPlayerLevelUI();

	// プレイヤーレベルを設定
	void SetLevel(int level);

	// 描画
	void Render()override;
private:
	CText* mpLevelText;
	CVector2 mBasePos;
};