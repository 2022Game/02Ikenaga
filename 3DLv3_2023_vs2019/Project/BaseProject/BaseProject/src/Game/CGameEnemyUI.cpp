#include "CGameEnemyUI.h"

// �R���X�g���N
CGameEnemyUI::CGameEnemyUI()
	:CTask(ETaskPriority::eUI, 0, ETaskPauseType::eGame)
{
	// HP�Q�[�W�쐬
	mpHpGauge = new CHpGauge(true);
	mpHpGauge->SetCenterRatio(CVector2(0.5f, 0.0f));

	// Lv.UI���쐬
	mpLvUI = new CLevelUI(true);
	mpLvUI->SetCenterRatio(CVector2(0.5f, 0.0f));
}

// �f�X�g���N�^
CGameEnemyUI::~CGameEnemyUI()
{
	// HP�Q�[�W�̍폜
	mpHpGauge->Kill();
	// LvUI�̍폜
	mpLvUI->Kill();
}

// HP��ݒ�
void CGameEnemyUI::SetHp(int hp)
{
	mpHpGauge->SetValue(hp);
}

// �ő�HP��ݒ�
void CGameEnemyUI::SetMaxHp(int maxHp)
{
	mpHpGauge->SetMaxValue(maxHp);
}

// HP�Q�[�W�̃I�t�Z�b�g�ʒu
void CGameEnemyUI::SetHpGaugeOffsetPos(const CVector& pos)
{
	mpHpGaugeOffsetPos = pos;
}

// UI�̃I�t�Z�b�g�ʒu���擾
const CVector& CGameEnemyUI::GetHpGaugeOffsetPos() const
{
	return mpHpGaugeOffsetPos;
}

// HP�Q�[�W���擾
CHpGauge* CGameEnemyUI::GetHpGauge() const
{
	return mpHpGauge;
}

// Lv��ݒ�
void CGameEnemyUI::SetLv(int lv)
{
	std::string lvStr = "Lv." + std::to_string(lv);
	mpLvUI->SetLevel(lvStr);
}

// Lv���擾
CLevelUI* CGameEnemyUI::GetLv()const
{
	return mpLvUI;
}

// Lv�̃I�t�Z�b�g�ʒu
void CGameEnemyUI::SetLvOffsetPos(const CVector& pos)
{
	mpLvOffsetPos = pos;
}

// UI�̃I�t�Z�b�g�ʒu���擾
const CVector& CGameEnemyUI::GetLvOffsetPos() const
{
	return mpLvOffsetPos;
}