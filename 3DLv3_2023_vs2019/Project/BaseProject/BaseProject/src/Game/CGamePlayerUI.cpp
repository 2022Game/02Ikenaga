#include "CGamePlayerUI.h"
#include "CPlayerNameUI.h"
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

// AvoidGauge���擾
CAvoidanceGauge* CGamePlayerUI::GetAvoidGauge() const
{
	return mpAvoidGauge;
}