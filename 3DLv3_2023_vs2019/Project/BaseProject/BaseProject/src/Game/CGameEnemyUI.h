#pragma once
#include "CTask.h"
#include "CHpGauge.h"
#include "CLevelUI.h"
#include "CEnemyLevelUI.h"
#include "CEnemyNameUI.h"

// �Q�[�����̃G�l�~�[UI
class CGameEnemyUI : public CTask
{
public:

	// �R���X�g���N
	CGameEnemyUI();

	// �f�X�g���N�^
	~CGameEnemyUI();

	// UI�̃I�t�Z�b�g�ʒu
	void SetUIoffSetPos(const CVector& pos);
	// UI�̃I�t�Z�b�g�ʒu���擾
	const CVector& GetUIoffSetPos() const;

	// �傫����ݒ�
	void SetScale(float scale);

	// HP��ݒ�
	void SetHp(int hp);
	// �ő��HP��ݒ�
	void SetMaxHp(int maxHp);
	// HP�Q�[�W���擾
	CHpGauge* GetHpGauge() const;

	// Lv��ݒ�
	void SetLv();
	// Lv���擾
	CLevelUI* GetLv() const;

	// ���x����ݒ�
	void SetEnemyLevel(int level);
	// ���x�����擾
	CEnemyLevelUI* GetLevel() const;

	// ���O��ݒ�
	void SetEnemyName(std::string name);
	// ���O���擾
	CEnemyNameUI* GetName() const;

private:
	// UI�֘A
	CVector mpUIoffSetPos;     // UI�̃I�t�Z�b�g�ʒu
	CHpGauge* mpHpGauge;       // HP�Q�[�W
	CLevelUI* mpLvUI;          // ���x����UI(Lv)
	CEnemyLevelUI* mpLevelUI;  // �G�̃��x��UI(����)
	CEnemyNameUI* mpNameUI;    // �G�̖��O
};