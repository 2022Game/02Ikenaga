#pragma once
#include "CBillBoardImage.h"

// ���x���A�b�vUI
class CLevelUpUI : public CBillBoardImage
{
public:
	// �R���X�g���N�^
	CLevelUpUI(float height);

	// �f�X�g���N�^
	~CLevelUpUI();

	// �������ݒ�
	void SetOwner(CCharaBase* owner) override;

	// �J�n
	void StartLevelUpUI();
	// �I��
	void EndLevelUpUI();

	// �X�V
	void Update() override;

private:

	// ���g�̃x�[�X�X�P�[���l���Z�o
	float CalcScale() const;

	float mElapsedTime;  // �o�ߎ���
	float mBaseScale;    // ������̃x�[�X�X�P�[���l
	float mHeight;       // ����
};