#pragma once
#include "CTask.h"
#include "CObjectBase.h"

class CDizzy;

// �߂܂��G�t�F�N�g
class CDizzyEffect : public CTask
{
public:
	// �R���X�g���N�^
	CDizzyEffect(CObjectBase* owner, const CMatrix* attach,
		const CVector& offsetPos = CVector::zero);
	// �f�X�g���N�^
	~CDizzyEffect();

	// �J�n
	void Start();
	// �I��
	void Stop();

	// �߂܂������Ă��邩�ǂ���
	bool IsDizzy() const;

	// �I�t�Z�b�g�ʒu��ݒ�
	void SetDizzyOffsetPos(const CVector& pos);

	// �X�V
	void Update() override;

private:
	// �߂܂����쐬
	void CreateDizzy();

	// �ʒu���擾
	CVector GetPos() const;

	// �G�t�F�N�g�̃��X�g
	std::list<CDizzy*> mDizzy;
	CObjectBase* mpOwner;		// ���̃G�t�F�N�g�̎�����
	const CMatrix* mpAttachMtx;	// �G�t�F�N�g���A�^�b�`����s��
	CVector mOffsetPos;	        // �I�t�Z�b�g�ʒu
	bool mIsDizzy;			    // �߂܂������Ă��邩
	int mStep;
};