#pragma once
#include "CTask.h"

class CPlayerNameUI;
class CHpGauge;
class CSpGauge;
class CAvoidanceGauge;
class CExpGauge;

// �Q�[�����̃v���C���[UI
class CGamePlayerUI : public CTask
{
public:

	// �R���X�g���N
	CGamePlayerUI();

	// �f�X�g���N�^
	~CGamePlayerUI();

	// �v���C���[����ݒ�
	void SetPlayerName(std::string name);

	// �ő�Hp��ݒ�
	void SetMaxHp(int maxHp);
	// ���݂�Hp��ݒ�
	void SetHp(int hp);

	// �ő�Sp��ݒ�
	void SetMaxSp(int maxSp);
	// ���݂�Sp��ݒ�
	void SetSp(int sp);

	// �ő�Avoid��ݒ�
	void SetAvoidMaxValue(int maxValue);
	// ���݂�Avoid��ݒ�
	void SetAvoidValue(int value);

	// �ő�Exp��ݒ�
	void SetMaxExp(int maxExp);
	// ���݂�Exp��ݒ�
	void SetExp(int exp);

	// AvoidGauge���擾
	CAvoidanceGauge* GetAvoidGauge()const;

private:
	CPlayerNameUI* mpNameUI;
	CHpGauge* mpHpGauge;
	CSpGauge* mpSpGauge;
	CAvoidanceGauge* mpAvoidGauge;
	CExpGauge* mpExpGauge;
};