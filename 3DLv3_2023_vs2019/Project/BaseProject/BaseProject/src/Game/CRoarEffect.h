#pragma once
#include "CTask.h"
#include "CObjectBase.h"

class CRoar;

// �Y���уG�t�F�N�g�N���X
class CRoarEffect : public CTask
{
public:
	// �R���X�g���N�^
	CRoarEffect(CObjectBase* owner, const CMatrix* attach,
		const CVector& offsetPos = CVector::zero,
		const CMatrix& offsetRot = CMatrix::identity);
	// �f�X�g���N�^
	~CRoarEffect();

	// �Y���т��J�n
	void Start();
	// �Y���т��~
	void Stop();

	// �Y���т����Ă��邩�ǂ���
	bool IsThrowing() const;

	// �Y���ю��̃I�t�Z�b�g�ʒu��ݒ�
	void SetThrowOffsetPos(const CVector& pos);
	// �Y���ю��̃I�t�Z�b�g��]�l��ݒ�
	void SetThrowOffsetRot(const CMatrix& rot);

	// �X�V
	void Update() override;

private:
	// �Y���т��쐬
	void CreateSlash();

	// �Y���т̈ʒu���擾
	CVector GetThrowPos() const;
	// �Y���т̕������擾
	CVector GetThrowDir() const;

	static CRoarEffect* spInstance;
	CObjectBase* mpOwner;		// ���̃G�t�F�N�g�̎�����
	const CMatrix* mpAttachMtx;	// �G�t�F�N�g���A�^�b�`����s��
	CVector mThrowOffsetPos;	// �Y���ю��̃I�t�Z�b�g�ʒu
	CMatrix mThrowOffsetRot;	// �Y���ю��̃I�t�Z�b�g��]�l
	float mElapsedTime;			// �o�ߎ��Ԍv���p
	bool mIsThrowing;			// �Y���т����Ă��邩
};