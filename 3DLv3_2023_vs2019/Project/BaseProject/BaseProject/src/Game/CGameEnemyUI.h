#pragma once
#include "CTask.h"
#include "CHpGauge.h"
#include "CLevelUI.h"

// �Q�[�����̃G�l�~�[UI
class CGameEnemyUI : public CTask
{
public:

	// �R���X�g���N
	CGameEnemyUI();

	// �f�X�g���N�^
	~CGameEnemyUI();

	// HP��ݒ�
	void SetHp(int hp);
	// �ő��HP��ݒ�
	void SetMaxHp(int maxHp);
	// HP�Q�[�W���擾
	CHpGauge* GetHpGauge() const;
	// HP�Q�[�W�̃I�t�Z�b�g�ʒu
	void SetHpGaugeOffsetPos(const CVector& pos);
	// HP�Q�[�W�̃I�t�Z�b�g�ʒu���擾
	const CVector& GetHpGaugeOffsetPos() const;

	// Lv��ݒ�
	void SetLv(int lv);
	// Lv���擾
	CLevelUI* GetLv()const;
	// Lv�̃I�t�Z�b�g�ʒu
	void SetLvOffsetPos(const CVector& pos);
	// Lv�̃I�t�Z�b�g�ʒu���擾
	const CVector& GetLvOffsetPos() const;

private:
	// UI�֘A
	CVector mpHpGaugeOffsetPos;  // HP�Q�[�W�̃I�t�Z�b�g�ʒu
	CHpGauge* mpHpGauge;    // HP�Q�[�W
	CLevelUI* mpLvUI;       // ���x����UI(Lv)
	CVector mpLvOffsetPos;  // Lv�̃I�t�Z�b�g�ʒu
};