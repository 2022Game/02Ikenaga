#pragma once
#include "CTask.h"
#include "CObjectBase.h"
#include <list>

class CBreath;

// �A�C�X�u���X�N���X
class CIceBreath : public CTask
{
public:

	// �R���X�g���N�^
	CIceBreath(CObjectBase* owner, const CMatrix* attach,
		const CVector& offsetPos = CVector::zero,
		const CMatrix& offsetRot = CMatrix::identity);

	// �f�X�g���N�^
	~CIceBreath();

	// ���ˊJ�n
	void Start();

	// ���˒�~
	void Stop();

	// �A�C�X�u���X�𔭎˂��Ă��邩�ǂ���
	bool IsThrowing() const;

	// ���ˎ��̃I�t�Z�b�g�ʒu��ݒ�
	void SetThrowOffsetPos(const CVector& pos);

	// ���ˎ��̃I�t�Z�b�g��]�l��ݒ�
	void SetThrowOffsetRot(const CMatrix& rot);

	// �X�V
	void Update() override;

private:

	// �u���X�̃G�t�F�N�g���쐬
	void CreateBreath();

	// �u���X�̔��ˈʒu���擾
	CVector GetThrowPos() const;
	// �u���X�̔��˕������擾
	CVector GetThrowDir() const;

	// ���˂����u���X�̃G�t�F�N�g�̃��X�g
	std::list<CBreath*> mBreath;

	CObjectBase* mpOwner;		// ���̃G�t�F�N�g�̎�����
	const CMatrix* mpAttachMtx;	// �G�t�F�N�g���A�^�b�`����s��
	CVector mThrowOffsetPos;	// ���ˎ��̃I�t�Z�b�g�ʒu
	CMatrix mThrowOffsetRot;	// ���ˎ��̃I�t�Z�b�g��]�l
	float mElapsedTime;			// �o�ߎ��Ԍv���p
	bool mIsThrowing;			// ���𔭎˂��Ă��邩
};