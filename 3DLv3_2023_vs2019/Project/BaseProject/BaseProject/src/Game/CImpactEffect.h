#pragma once
#include "CTask.h"
#include "CObjectBase.h"

class CImpact;

// �Ռ��G�t�F�N�g�N���X
class CImpactEffect : public CTask
{
public:
	// �R���X�g���N�^
	CImpactEffect(CObjectBase* owner, const CMatrix* attach,
		const CVector& offsetPos = CVector::zero,
		const CMatrix& offsetRot = CMatrix::identity);
	// �f�X�g���N�^
	~CImpactEffect();

	// �Ռ����J�n
	void Start();
	// �Ռ����~
	void Stop();

	// �Ռ������Ă��邩�ǂ���
	bool IsThrowing() const;

	// �Ռ����̃I�t�Z�b�g�ʒu��ݒ�
	void SetThrowOffsetPos(const CVector& pos);
	// �Ռ����̃I�t�Z�b�g��]�l��ݒ�
	void SetThrowOffsetRot(const CMatrix& rot);

	// �X�V
	void Update() override;

private:
	// �Ռ����쐬
	void CreateImpact();

	// �Ռ��̈ʒu���擾
	CVector GetThrowPos() const;
	// �Ռ��̕������擾
	CVector GetThrowDir() const;

	// ���˂����Ռ��̃G�t�F�N�g�̃��X�g
	std::list<CImpact*> mImpact;
	CObjectBase* mpOwner;		// ���̃G�t�F�N�g�̎�����
	const CMatrix* mpAttachMtx;	// �G�t�F�N�g���A�^�b�`����s��
	CVector mThrowOffsetPos;	// �Ռ����̃I�t�Z�b�g�ʒu
	CMatrix mThrowOffsetRot;	// �Ռ����̃I�t�Z�b�g��]�l
	float mElapsedTime;			// �o�ߎ��Ԍv���p
	bool mIsThrowing;			// �Ռ������Ă��邩
};