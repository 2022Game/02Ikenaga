#pragma once
#include "CBillBoardImage.h"

// �q�b�g
class CHit : public CBillBoardImage
{
public:
	// �R���X�g���N�^
	CHit(ETag tag, float size,float height);

	// �f�X�g���N�^
	~CHit();

	// �������ݒ�
	void SetOwner(CCharaBase* owner) override;

	// �q�b�g�G�t�F�N�g�J�n
	void StartHitEffect();
	// �q�b�g�G�t�F�N�g�I��
	void EndHitEffect();

	// �X�V
	void Update() override;

private:

	// ���g�̃x�[�X�X�P�[���l���Z�o
	float CalcScale() const;

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
	float mSize;         // �傫��
	float mHeight;       // ����
};