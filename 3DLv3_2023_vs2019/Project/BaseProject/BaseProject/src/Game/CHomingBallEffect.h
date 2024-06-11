#pragma once
#include "CTask.h"
#include "CObjectBase.h"

class CHomingBall;

// �z�[�~���O�{�[���̃G�t�F�N�g�N���X
class CHomingBallEffect : public CTask
{
public:
	// �R���X�g���N�^
	CHomingBallEffect(CObjectBase* owner, const CMatrix* attach,
		const CVector& offsetPos = CVector::zero,
		const CMatrix& offsetRot = CMatrix::identity);
	// �f�X�g���N�^
	~CHomingBallEffect();

	// �z�[�~���O�{�[���̔��˂��J�n
	void Start();
	// �z�[�~���O�{�[���̔��˂��~
	void Stop();

	// �z�[�~���O�{�[���𔭎˂����Ă��邩�ǂ���
	bool IsThrowing() const;

	// �z�[�~���O�{�[���̔��ˎ��̃I�t�Z�b�g�ʒu��ݒ�
	void SetThrowOffsetPos(const CVector& pos);
	// �z�[�~���O�{�[���̔��ˎ��̃I�t�Z�b�g��]�l��ݒ�
	void SetThrowOffsetRot(const CMatrix& rot);

	// �X�V
	void Update() override;

private:
	// �z�[�~���O�{�[�����쐬
	void CreateLightningBall();

	// �z�[�~���O�{�[���̔��˂̈ʒu���擾
	CVector GetThrowPos() const;
	// �z�[�~���O�{�[���̔��˂̕������擾
	CVector GetThrowDir() const;

	// ���˂����z�[�~���O�{�[���̃G�t�F�N�g�̃��X�g
	std::list<CHomingBall*> mHomingBall;
	CObjectBase* mpOwner;		// ���̃G�t�F�N�g�̎�����
	const CMatrix* mpAttachMtx;	// �G�t�F�N�g���A�^�b�`����s��
	CVector mThrowOffsetPos;	// �z�[�~���O�{�[�����ˎ��̃I�t�Z�b�g�ʒu
	CMatrix mThrowOffsetRot;	// �z�[�~���O�{�[�����ˎ��̃I�t�Z�b�g��]�l
	float mElapsedTime;			// �o�ߎ��Ԍv���p
	bool mIsThrowing;			// �z�[�~���O�{�[���𔭎˂����Ă��邩
};