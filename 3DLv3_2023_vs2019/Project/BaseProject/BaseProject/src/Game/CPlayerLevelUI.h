#pragma once
#include "CUIBase.h"

class CText;

// プレイヤーレベルのUI
class CPlayerLevelUI : public CUIBase
{
public:

	// コンストラク
	CPlayerLevelUI();

	// デストラクタ
	~CPlayerLevelUI();

	// プレイヤーレベルを設定
	void SetPlayerLevel(int level);

	// 更新
	void Update()override;

	// 描画
	void Render()override;
private:
	CText* mpLevelText;
	CVector2 mBasePos;
};