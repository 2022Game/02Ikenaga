#pragma once
#include "CTask.h"
#include "CObjectBase.h"

class CBeam;

// �r�[���G�t�F�N�g�N���X
class CBeamEffect : public CTask
{
public:
	// �R���X�g���N�^
	CBeamEffect(CObjectBase* owner, const CMatrix* attach,
		const CVector& offsetPos = CVector::zero,
		const CMatrix& offsetRot = CMatrix::identity);
	// �f�X�g���N�^
	~CBeamEffect();

	// �r�[�����J�n
	void Start();
	// �r�[�����~
	void Stop();

	// �r�[�������Ă��邩�ǂ���
	bool IsThrowing() const;

	// �r�[�����̃I�t�Z�b�g�ʒu��ݒ�
	void SetThrowOffsetPos(const CVector& pos);
	// �r�[�����̃I�t�Z�b�g��]�l��ݒ�
	void SetThrowOffsetRot(const CMatrix& rot);

	// �X�V
	void Update() override;

private:
	// �r�[�����쐬
	void CreateSlash();

	// �r�[���̈ʒu���擾
	CVector GetThrowPos() const;
	// �r�[���̕������擾
	CVector GetThrowDir() const;

	CObjectBase* mpOwner;		// ���̃G�t�F�N�g�̎�����
	const CMatrix* mpAttachMtx;	// �G�t�F�N�g���A�^�b�`����s��
	CVector mThrowOffsetPos;	// �r�[�����̃I�t�Z�b�g�ʒu
	CMatrix mThrowOffsetRot;	// �r�[�����̃I�t�Z�b�g��]�l
	float mElapsedTime;			// �o�ߎ��Ԍv���p
	bool mIsThrowing;			// �Y���т����Ă��邩
};