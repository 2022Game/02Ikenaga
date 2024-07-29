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

	// Level���쐬
	mpLevelUI = new CEnemyLevelUI(true);
	mpLevelUI->SetCenterRatio(CVector2(0.5f, 0.0f));

	// ���O���쐬
	mpNameUI = new CEnemyNameUI(true);
	mpNameUI->SetCenterRatio(CVector2(0.5f, 0.0f));
}

// �f�X�g���N�^
CGameEnemyUI::~CGameEnemyUI()
{
	// HP�Q�[�W�̍폜
	mpHpGauge->Kill();
	// LvUI�̍폜
	mpLvUI->Kill();
	// LevelUI�̍폜
	mpLevelUI->Kill();
	// ���O���폜
	mpNameUI->Kill();
}

// UI�̃I�t�Z�b�g�ʒu
void CGameEnemyUI::SetUIoffSetPos(const CVector& pos)
{
	mpUIoffSetPos = pos;
}

// UI�̃I�t�Z�b�g�ʒu���擾
const CVector& CGameEnemyUI::GetUIoffSetPos() const
{
	return mpUIoffSetPos;
}

// �傫����ݒ�
void CGameEnemyUI::SetScale(float scale)
{
	mpHpGauge->SetScale(scale);
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

// HP�Q�[�W���擾
CHpGauge* CGameEnemyUI::GetHpGauge() const
{
	return mpHpGauge;
}

// Lv��ݒ�
void CGameEnemyUI::SetLv()
{
	std::string lvStr = "Lv.";
	mpLvUI->SetLevel(lvStr);
}

// Lv���擾
CLevelUI* CGameEnemyUI::GetLv()const
{
	return mpLvUI;
}

// ���x����ݒ�
void CGameEnemyUI::SetEnemyLevel(int level)
{
	mpLevelUI->SetEnemyLevel(level);
}

// ���x�����擾
CEnemyLevelUI* CGameEnemyUI::GetLevel() const
{
	return mpLevelUI;
}

// ���O��ݒ�
void CGameEnemyUI::SetEnemyName(std::string name)
{
	mpNameUI->SetEnemyName(name);
}

// ���O���擾
CEnemyNameUI* CGameEnemyUI::GetName() const
{
	return mpNameUI;
}
