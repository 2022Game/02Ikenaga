#pragma once
#include "CTask.h"
#include "CObjectBase.h"

class CSlashing;

// �X���b�V���N���X
class CSlash : public CTask
{
public:
	// �R���X�g���N�^
	CSlash(CObjectBase* owner, const CMatrix* attach,
		const CVector& offsetPos = CVector::zero,
		const CMatrix& offsetRot = CMatrix::identity);
	// �f�X�g���N�^
	~CSlash();

	// �X���b�V���𔭎ˊJ�n
	void Start();
	// �X���b�V�𔭎˒�~
	void Stop();

	// �X���b�V���𔭎˂��Ă��邩�ǂ���
	bool IsThrowing() const;

	// �X���b�V���̔��ˎ��̃I�t�Z�b�g�ʒu��ݒ�
	void SetThrowOffsetPos(const CVector& pos);
	// �X���b�V���̔��ˎ��̃I�t�Z�b�g��]�l��ݒ�
	void SetThrowOffsetRot(const CMatrix& rot);

	// �X�V
	void Update() override;

private:
	// �X���b�V�����쐬
	void CreateSlash();

	// �X���b�V���̔��ˈʒu���擾
	CVector GetThrowPos() const;
	// �X���b�V���̔��˕������擾
	CVector GetThrowDir() const;

	static CSlash* spInstance;
	CObjectBase* mpOwner;		// ���̃G�t�F�N�g�̎�����
	const CMatrix* mpAttachMtx;	// �G�t�F�N�g���A�^�b�`����s��
	CVector mThrowOffsetPos;	// ���ˎ��̃I�t�Z�b�g�ʒu
	CMatrix mThrowOffsetRot;	// ���ˎ��̃I�t�Z�b�g��]�l
	float mElapsedTime;			// �o�ߎ��Ԍv���p
	bool mIsThrowing;			// �X���b�V���𔭎˂��Ă��邩
};