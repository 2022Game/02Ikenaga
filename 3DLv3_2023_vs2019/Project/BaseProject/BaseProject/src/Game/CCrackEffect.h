#pragma once
#include "CTask.h"
#include "CObjectBase.h"

class CCrack;

// �Ђъ���G�t�F�N�g�N���X
class CCrackEffect : public CTask
{
public:
	// �R���X�g���N�^
	CCrackEffect(CObjectBase* owner, const CMatrix* attach,
		const CVector& offsetPos = CVector::zero,
		const CMatrix& offsetRot = CMatrix::identity);
	// �f�X�g���N�^
	~CCrackEffect();

	// �Ђъ�����J�n
	void Start();
	// �Ђъ�����~
	void Stop();

	// �Ђъ�������Ă��邩�ǂ���
	bool IsThrowing() const;

	// �Ђъ��ꎞ�̃I�t�Z�b�g�ʒu��ݒ�
	void SetThrowOffsetPos(const CVector& pos);
	// �Ђъ��ꎞ�̃I�t�Z�b�g��]�l��ݒ�
	void SetThrowOffsetRot(const CMatrix& rot);

	// �X�V
	void Update() override;

private:
	// �Ђъ�����쐬
	void CreateSlash();

	// �Ђъ���̈ʒu���擾
	CVector GetThrowPos() const;
	// �Ђъ���̕������擾
	CVector GetThrowDir() const;

	static CCrackEffect* spInstance;
	CObjectBase* mpOwner;		// ���̃G�t�F�N�g�̎�����
	const CMatrix* mpAttachMtx;	// �G�t�F�N�g���A�^�b�`����s��
	CVector mThrowOffsetPos;	// �Ђъ��ꎞ�̃I�t�Z�b�g�ʒu
	CMatrix mThrowOffsetRot;	// �Ђъ��ꎞ�̃I�t�Z�b�g��]�l
	float mElapsedTime;			// �o�ߎ��Ԍv���p
	bool mIsThrowing;			// �Ђъ�������Ă��邩
};