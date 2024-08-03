#pragma once
#include "CBillBoardImage.h"

// �v���C���[�̃q�b�g�G�t�F�N�g
class CPlayerHit : public CBillBoardImage
{
public:
	// �R���X�g���N�^
	CPlayerHit(float height);

	// �f�X�g���N�^
	~CPlayerHit();

	// �������ݒ�
	void SetOwner(CCharaBase* owner) override;

	// �J�n
	void StartHit(); 
	// �I��
	void EndHit();

	// �X�V
	void Update() override;

private:

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

	// ���g�̃x�[�X�X�P�[���l���Z�o
	float CalcScale() const;

	float mElapsedTime;  // �o�ߎ���
	float mBaseScale;    // ������̃x�[�X�X�P�[���l
	float mHeight;       // ����
};