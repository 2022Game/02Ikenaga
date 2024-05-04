#pragma once
#include "CTask.h"
#include "CObjectBase.h"
#include "CColliderSphere.h"

class CWave;

// �g���G�t�F�N�g�N���X
class CWaveEffect : public CTask
{
public:
	// �R���X�g���N�^
	CWaveEffect(CObjectBase* owner, const CMatrix* attach,
		const CVector& offsetPos = CVector::zero,
		const CMatrix& offsetRot = CMatrix::identity);
	// �f�X�g���N�^
	~CWaveEffect();

	// �g�����J�n
	void Start();
	// �g�����~
	void Stop();

	// �g�������Ă��邩�ǂ���
	bool IsThrowing() const;

	// �g�����̃I�t�Z�b�g�ʒu��ݒ�
	void SetThrowOffsetPos(const CVector& pos);
	// �g�����̃I�t�Z�b�g��]�l��ݒ�
	void SetThrowOffsetRot(const CMatrix& rot);

	// �X�V
	void Update() override;

private:
	// �g�����쐬
	void CreateWave();

	// �g���̈ʒu���擾
	CVector GetThrowPos() const;
	// �g���̕������擾
	CVector GetThrowDir() const;

	static CWaveEffect* spInstance;
	CObjectBase* mpOwner;		// ���̃G�t�F�N�g�̎�����
	const CMatrix* mpAttachMtx;	// �G�t�F�N�g���A�^�b�`����s��
	CVector mThrowOffsetPos;	// �g�����̃I�t�Z�b�g�ʒu
	CMatrix mThrowOffsetRot;	// �g�����̃I�t�Z�b�g��]�l
	float mElapsedTime;			// �o�ߎ��Ԍv���p
	bool mIsThrowing;			// �g�������Ă��邩
};