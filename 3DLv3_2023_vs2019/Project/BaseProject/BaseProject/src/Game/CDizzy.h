#pragma once
#include "CBillBoardImage.h"

// �߂܂�
class CDizzy : public CBillBoardImage
{
public:
	// �R���X�g���N�^
	CDizzy();
	// �f�X�g���N�^
	~CDizzy();

	// �ʒu��ݒ�
	void Setup(const CVector& pos);

	// �J���[��ݒ�
	void SetColor(const CColor& color) override;
	// �u�����h�^�C�v��ݒ�
	void SetBlendType(EBlend type);

	// �폜�t���O�������Ă��邩�ǂ���
	bool IsDeath() const;

	// �X�V
	void Update() override;

private:
	// �A�j���[�V�����f�[�^
	static TexAnimData msAnimData;
	float mElapsedTime;	// �o�ߎ���
	bool mIsDeath;		// �폜�t���O
};