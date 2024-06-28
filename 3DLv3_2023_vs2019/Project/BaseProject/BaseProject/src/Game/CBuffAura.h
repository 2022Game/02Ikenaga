#pragma once
#include "CBillBoardImage.h"

// �o�t�I�[��
class CBuffAura : public CBillBoardImage
{
public:
	// �R���X�g���N�^
	CBuffAura();

	// �f�X�g���N�^
	~CBuffAura();

	// �������ݒ�
	void SetOwner(CCharaBase* owner) override;

	// �J���[��ݒ�
	void SetColor(const CColor& color) override;

	// �u�����h�^�C�v��ݒ�
	void SetBlendType(EBlend type);

	// �o�t�I�[���J�n
	void StartAura();
	// �o�t�I�[���I��
	void EndAura();

	// �X�V
	void Update() override;

private:

	// ���g�̃x�[�X�X�P�[���l���Z�o
	float CalcScaleXZ() const;

	float CalcScaleY() const;

	// �ҋ@��
	void UpdateIdle();
	// �J�n
	void UpdateStart();
	// �I���҂�
	void UpdateWait();
	// �I��
	void UpdateEnd();

	// ���
	enum EState
	{
		Idle,   // �ҋ@��
		Start,  // �J�n
		Wait,   // �I���҂�
		End,    // �I��
	};
	void ChangeState(EState state);

	EState mState;       // ���݂̏��

	float mElapsedTime;  // �o�ߎ���
	float mBaseScale;    // ������̃x�[�X�X�P�[���l
};