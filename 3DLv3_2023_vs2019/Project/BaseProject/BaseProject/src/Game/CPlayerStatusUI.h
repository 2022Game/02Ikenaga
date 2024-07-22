#pragma once
#include "CTask.h"
#include "CImage.h"

class CText;
class CPlayerLevelUI;
class CPlayerExpUI;
class CPlayerMaxHpUI;
class CPlayerHpUI;

// �v���C���[�̃X�e�[�^�XUI
class CPlayerStatusUI : public CTask
{
public:
	// �R���X�g���N
	CPlayerStatusUI();
	// �f�X�g���N�^
	~CPlayerStatusUI();

	// �I�[�v��
	void Open();
	// �N���[�Y
	void Close();
	// �I�[�v�����邩�ǂ���
	bool IsOpened() const;

	// ���߂�
	void Decide(int select);

	// �v���C���[���x����ݒ�
	void SetLevel(int level);

	// �v���C���[�o���l��ݒ�
	void SetExp(int exp);

	// �v���C���[�ő�HP��ݒ�
	void SetMaxHp(int maxHp);
	// �v���C���[HP��ݒ�
	void SetHp(int hp);

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
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
	CText* mpLine2;       // ��2
	CText* mpLine3;       // ��3
	// UI�֘A
	CPlayerLevelUI* mpLevelUI;  // ���x��UI
	CPlayerExpUI* mpExpUI;      // �o���l
	CPlayerMaxHpUI* mpMaxHpUI;  // �ő�HpUI
	CPlayerHpUI* mpHpUI;        // HpUI
	bool mIsOpened;
};