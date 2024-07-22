#include "CGamePlayerUI.h"
#include "CPlayerNameUI.h"
#include "CLevelUI.h"
#include "CPlayerLevelUI.h"
#include "CHpUI.h"
#include "CSpUI.h"
#include "CExpUI.h"
#include "CHpGauge.h"
#include "CSpGauge.h"
#include "CAvoidanceGauge.h"
#include "CExpGauge.h"

// �R���X�g���N
CGamePlayerUI::CGamePlayerUI()
	:CTask(ETaskPriority::eUI,0,ETaskPauseType::eGame)
{
	// �v���C���[����UI���쐬
	mpNameUI = new CPlayerNameUI();

	// Lv.UI���쐬
	mpLevelUI = new CLevelUI(false);
	// �v���C���[���x����UI���쐬
	mpPlayerLevelUI = new CPlayerLevelUI(50.0f,32.0f);
	mpPlayerLevelUI->SetShow(true);

	// HpUI���쐬
	mpHpUI = new CHpUI();

	// SpUI���쐬
	mpSpUI = new CSpUI();

	// ExpUI���쐬
	mpExpUI = new CExpUI();

	// �v���C���[��HP�Q�[�W���쐬
	mpHpGauge = new CHpGauge(false);
	mpHpGauge->SetPos(10.0f, 63.0f);
	mpHpGauge->SetShow(true);

	// �v���C���[��SP�Q�[�W���쐬
	mpSpGauge = new CSpGauge();
	mpSpGauge->SetPos(10.0f, 103.5f);

	// �v���C���[�̉���Q�[�W�쐬
	mpAvoidGauge = new CAvoidanceGauge(true);
	mpAvoidGauge->SetCenterRatio(CVector2(0.5f, 0.0f));

	// �v���C���[��Exp�Q�[�W�쐬
	mpExpGauge = new CExpGauge();
	mpExpGauge->SetPos(320.0f, 700.0f);
}

// �f�X�g���N�^
CGamePlayerUI::~CGamePlayerUI()
{
	// UI�֘A�̍폜
	mpNameUI->Kill();
	mpLevelUI->Kill();
	mpPlayerLevelUI->Kill();
	mpHpUI->Kill();
	mpSpUI->Kill();
	mpExpUI->Kill();
	// �Q�[�W�֘A�̍폜
	mpHpGauge->Kill();
	mpSpGauge->Kill();
	mpAvoidGauge->Kill();
	mpExpGauge->Kill();
}

// �v���C���[����ݒ�
void CGamePlayerUI::SetPlayerName(std::string name)
{
	mpNameUI->SetPlayerName(name);
}

// Lv��ݒ�
void CGamePlayerUI::SetLevel(std::string lv)
{
	mpLevelUI->SetLevel(lv);
}

// �v���C���[���x����ݒ�
void CGamePlayerUI::SetPlayerLevel(int level)
{
	mpPlayerLevelUI->SetLevel(level);
}

// �ő�Hp��ݒ�
void CGamePlayerUI::SetMaxHp(int maxHp)
{
	mpHpGauge->SetMaxValue(maxHp);
}

// ���݂�Hp��ݒ�
void CGamePlayerUI::SetHp(int hp)
{
	mpHpGauge->SetValue(hp);
}

// Hp��\��
void CGamePlayerUI::SetHpText(std::string hpText)
{
	mpHpUI->SetHpText(hpText);
}

// �ő�Sp��ݒ�
void CGamePlayerUI::SetMaxSp(int maxSp)
{
	mpSpGauge->SetMaxValue(maxSp);
}

// ���݂�Sp��ݒ�
void CGamePlayerUI::SetSp(int sp)
{
	mpSpGauge->SetValue(sp);
}

// Sp��\��
void CGamePlayerUI::SetSpText(std::string spText)
{
	mpSpUI->SetSpText(spText);
}

// �ő�Avoid��ݒ�
void CGamePlayerUI::SetAvoidMaxValue(int maxValue)
{
	mpAvoidGauge->SetMaxValue(maxValue);
}

// ���݂�Avoid��ݒ�
void CGamePlayerUI::SetAvoidValue(int value)
{
	mpAvoidGauge->SetValue(value);
}

// �ő�Exp��ݒ�
void CGamePlayerUI::SetMaxExp(int maxExp)
{
	mpExpGauge->SetMaxValue(maxExp);
}

// ���݂�Exp��ݒ�
void CGamePlayerUI::SetExp(int exp)
{
	mpExpGauge->SetValue(exp);
}

// Exp��\��
void CGamePlayerUI::SetExpText(std::string expText)
{
	mpExpUI->SetExpText(expText);
}

// AvoidGauge���擾
CAvoidanceGauge* CGamePlayerUI::GetAvoidGauge() const
{
	return mpAvoidGauge;
}