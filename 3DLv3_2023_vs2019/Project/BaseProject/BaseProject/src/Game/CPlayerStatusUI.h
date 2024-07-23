#pragma once
#include "CTask.h"
#include "CImage.h"

class CText;
class CPlayerLevelUI;
class CPlayerMaxExpUI;
class CPlayerExpUI;
class CPlayerMaxHpUI;
class CPlayerHpUI;
class CPlayerMaxSpUI;
class CPlayerSpUI;
class CPlayerPowerUI;
class CPlayerDefenseUI;
class CPlayerScaleUI;

// �v���C���[�̃X�e�[�^�XUI
class CPlayerStatusUI : public CTask
{
public:
	// �R���X�g���N
	CPlayerStatusUI();
	// �f�X�g���N�^
	~CPlayerStatusUI();

	// �\���ݒ�
	void SetShow(bool show)override;

	// �I�[�v��
	void Open();
	// �N���[�Y
	void Close();
	// �I�[�v�����邩�ǂ���
	bool IsOpened() const;

	// ���߂�
	void Decide(int select);

	// �v���C���[�̃��x����ݒ�
	void SetLevel(int level);

	// �v���C���[�̍ő�o���l��ݒ�
	void SetMaxExp(int maxExp);
	// �v���C���[�̌o���l��ݒ�
	void SetExp(int exp);

	// �v���C���[�̍ő�HP��ݒ�
	void SetMaxHp(int maxHp);
	// �v���C���[��HP��ݒ�
	void SetHp(int hp);

	// �v���C���[�̍ő�SP��ݒ�
	void SetMaxSp(int maxSp);
	// �v���C���[��SP��ݒ�
	void SetSp(int sp);

	// �v���C���[�̍U���͂�ݒ�
	void SetPower(int power);

	// �v���C���[�̖h��͂�ݒ�
	void SetDefense(int defense);

	// �v���C���[�̑傫����ݒ�
	void SetScale(float scale);

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
	// ���݂̃v���C���[�̃X�e�[�^�X�𔽉f
	void ApplyPlayerStatus();

	// �w�i
	CImage* mpBackground;
	// �X�e�[�^�X
	CImage* mpStatus;
	// �e�L�X�g�֘A
	CText* mpLevelText;   // ���x���̃e�L�X�g
	CText* mpExpText;     // �o���l�̃e�L�X�g
	CText* mpHpText;      // HP�̃e�L�X�g
	CText* mpSpText;      // SP�̃e�L�X�g
	CText* mpAttackText;  // �U���͂̃e�L�X�g
	CText* mpDefenseText; // �h��͂̃e�L�X�g
	CText* mpSizeText;    // �傫���̃e�L�X�g
	CText* mpLine;        // ��
	CText* mpLine2;       // ��2
	CText* mpLine3;       // ��3
	CText* mpDetail;      // �ڍ�
	// UI�֘A
	CPlayerLevelUI* mpLevelUI;     // ���x��
	CPlayerMaxExpUI* mpMaxExpUI;   // �o���l
	CPlayerExpUI* mpExpUI;         // �o���l
	CPlayerMaxHpUI* mpMaxHpUI;     // �ő�Hp
	CPlayerHpUI* mpHpUI;           // Hp
	CPlayerMaxSpUI* mpMaxSpUI;     // �ő�Sp
	CPlayerSpUI* mpSpUI;           // Sp
	CPlayerPowerUI* mpPowerUI;     // �U����
	CPlayerDefenseUI* mpDefenseUI; // �h���
	CPlayerScaleUI* mpScaleUI;     // �傫��
	bool mIsOpened;
};