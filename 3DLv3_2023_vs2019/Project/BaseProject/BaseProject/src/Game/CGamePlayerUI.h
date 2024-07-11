#pragma once
#include "CTask.h"

class CPlayerNameUI;
class CLevelUI;
class CPlayerLevelUI;
class CHpGauge;
class CSpGauge;
class CAvoidanceGauge;
class CExpGauge;
class CHpUI;
class CSpUI;
class CExpUI;

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

	// Lv��ݒ�
	void SetLevel(std::string lv);
	// �v���C���[���x����ݒ�
	void SetPlayerLevel(int level);

	// �ő�Hp��ݒ�
	void SetMaxHp(int maxHp);
	// ���݂�Hp��ݒ�
	void SetHp(int hp);
	// Hp��\��
	void SetHpText(std::string hpText);

	// �ő�Sp��ݒ�
	void SetMaxSp(int maxSp);
	// ���݂�Sp��ݒ�
	void SetSp(int sp);
	// Sp��\��
	void SetSpText(std::string spText);

	// �ő�Avoid��ݒ�
	void SetAvoidMaxValue(int maxValue);
	// ���݂�Avoid��ݒ�
	void SetAvoidValue(int value);

	// �ő�Exp��ݒ�
	void SetMaxExp(int maxExp);
	// ���݂�Exp��ݒ�
	void SetExp(int exp);
	// Exp��\��
	void SetExpText(std::string expText);

	// AvoidGauge���擾
	CAvoidanceGauge* GetAvoidGauge()const;

private:
	// UI�֘A
	CPlayerNameUI* mpNameUI;          // ���O��UI
	CLevelUI* mpLevelUI;              // ���x����UI(Lv)
	CPlayerLevelUI* mpPlayerLevelUI;  // �v���C���[�̃��x��UI
	CHpUI* mpHpUI;                    // Hp��UI
	CSpUI* mpSpUI;                    // Sp��UI
	CExpUI* mpExpUI;                  // Exp��UI

	// �Q�[�W�֘A
	CHpGauge* mpHpGauge;            // Hp�Q�[�W
	CSpGauge* mpSpGauge;            // Sp�Q�[�W
	CAvoidanceGauge* mpAvoidGauge;  // ����Q�[�W
	CExpGauge* mpExpGauge;          // Exp�Q�[�W
};