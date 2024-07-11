#pragma once
#include "CTask.h"

class CLevelUI;

// ゲーム中のエネミーUI
class CGameEnemyUI : public CTask
{
public:

	// コンストラク
	CGameEnemyUI();

	// デストラクタ
	~CGameEnemyUI();

	// Lvを設定
	void SetLv(std::string lv);

	// Lvを取得
	CLevelUI* GetLv()const;

private:
	// UI関連
	CLevelUI* mpLvUI;  // レベルのUI(Lv)
};