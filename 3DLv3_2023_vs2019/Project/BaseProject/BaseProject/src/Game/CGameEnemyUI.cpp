#include "CGameEnemyUI.h"
#include "CLevelUI.h"

// コンストラク
CGameEnemyUI::CGameEnemyUI()
	:CTask(ETaskPriority::eUI, 0, ETaskPauseType::eGame)
{
	// Lv.UIを作成
	mpLvUI = new CLevelUI(true);
	mpLvUI->SetCenterRatio(CVector2(0.5f, 0.0f));
}

// デストラクタ
CGameEnemyUI::~CGameEnemyUI()
{
	mpLvUI->Kill();
}

// Lvを設定
void CGameEnemyUI::SetLv(std::string lv)
{
	mpLvUI->SetLevel(lv);
}

CLevelUI* CGameEnemyUI::GetLv()const
{
	return mpLvUI;
}