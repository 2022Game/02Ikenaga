#include "CGameEnemyUI.h"
#include "CLevelUI.h"

// �R���X�g���N
CGameEnemyUI::CGameEnemyUI()
	:CTask(ETaskPriority::eUI, 0, ETaskPauseType::eGame)
{
	// Lv.UI���쐬
	mpLvUI = new CLevelUI(true);
	mpLvUI->SetCenterRatio(CVector2(0.5f, 0.0f));
}

// �f�X�g���N�^
CGameEnemyUI::~CGameEnemyUI()
{
	mpLvUI->Kill();
}

// Lv��ݒ�
void CGameEnemyUI::SetLv(std::string lv)
{
	mpLvUI->SetLevel(lv);
}

CLevelUI* CGameEnemyUI::GetLv()const
{
	return mpLvUI;
}