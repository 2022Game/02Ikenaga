#pragma once
#include "CTask.h"
#include "CObjectBase.h"

class CDrain;

// �h���C���̃G�t�F�N�g�N���X
class CDrainEffect : public CTask
{
public:
	// �R���X�g���N�^
	CDrainEffect(CObjectBase* owner, const CMatrix* attach,
		const CVector& offsetPos = CVector::zero,
		const CMatrix& offsetRot = CMatrix::identity);
	// �f�X�g���N�^
	~CDrainEffect();

	// �h���C���̔��˂��J�n
	void Start();
	// �h���C���̔��˂��~
	void Stop();

	// �h���C���𔭎˂����Ă��邩�ǂ���
	bool IsThrowing() const;

	// �h���C���̔��ˎ��̃I�t�Z�b�g�ʒu��ݒ�
	void SetThrowOffsetPos(const CVector& pos);
	// �h���C���̔��ˎ��̃I�t�Z�b�g��]�l��ݒ�
	void SetThrowOffsetRot(const CMatrix& rot);

	// �X�V
	void Update() override;

private:
	// �h���C�����쐬
	void CreateDrain();

	// �h���C���̔��˂̈ʒu���擾
	CVector GetThrowPos() const;
	// �h���C���̔��˂̕������擾
	CVector GetThrowDir() const;

	CObjectBase* mpOwner;		// ���̃G�t�F�N�g�̎�����
	const CMatrix* mpAttachMtx;	// �G�t�F�N�g���A�^�b�`����s��
	CVector mThrowOffsetPos;	// �h���C�����ˎ��̃I�t�Z�b�g�ʒu
	CMatrix mThrowOffsetRot;	// �h���C�����ˎ��̃I�t�Z�b�g��]�l
	float mElapsedTime;			// �o�ߎ��Ԍv���p
	bool mIsThrowing;			// �h���C���𔭎˂����Ă��邩
};