#include "CDizzyEffect.h"
#include "CDizzy.h"
#include "Maths.h"
#include "CCharaBase.h"

// �R���X�g���N�^
CDizzyEffect::CDizzyEffect(CObjectBase* owner, const CMatrix* attach,
	const CVector& offsetPos)
	: mpOwner(owner)
	, mpAttachMtx(attach)
	, mOffsetPos(offsetPos)
	, mIsDizzy(false)
	, mStep(0)
{
}

// �f�X�g���N�^
CDizzyEffect::~CDizzyEffect()
{
}

// �J�n
void CDizzyEffect::Start()
{
	mIsDizzy = true;
	mStep = 0;
}

// ��~
void CDizzyEffect::Stop()
{
	mIsDizzy = false;
}

// �߂܂������Ă��邩�ǂ���
bool CDizzyEffect::IsDizzy() const
{
	return mIsDizzy;
}

// �I�t�Z�b�g�ʒu��ݒ�
void CDizzyEffect::SetDizzyOffsetPos(const CVector& pos)
{
	mOffsetPos = pos;
}

// �ʒu���擾
CVector CDizzyEffect::GetPos() const
{
	// �A�^�b�`����s�񂪐ݒ肳��Ă���ꍇ�́A�s��̍��W��Ԃ�
	if (mpAttachMtx != nullptr)
	{
		CVector pos = mpAttachMtx->Position();
		pos += mpAttachMtx->VectorX().Normalized() * mOffsetPos.X()
			+ mpAttachMtx->VectorY().Normalized() * mOffsetPos.Y()
			+ mpAttachMtx->VectorZ().Normalized() * mOffsetPos.Z();
		return pos;
	}
	// �����傪�ݒ肳��Ă���ꍇ�́A������̍��W��Ԃ�
	else if (mpOwner != nullptr)
	{
		return mpOwner->Position() + (mpOwner->Rotation() * mOffsetPos);
	}

	return CVector::zero;
}

// �߂܂����쐬
void CDizzyEffect::CreateDizzy()
{
	// �߂܂����쐬
	CDizzy* dizzy = new CDizzy();
	CCharaBase* owner = dynamic_cast<CCharaBase*>(mpOwner);
	if (owner != nullptr)
	{
		dizzy->SetOwner(owner);
	}

	// �ʒu���擾
	CVector pos = GetPos();// + CVector(0.0f, 10.0f, 0.0f);
	dizzy->Setup(pos);
	dizzy->SetSize(CVector2(20, 20));

	mDizzy.push_back(dizzy);
}

// �X�V
void CDizzyEffect::Update()
{
	if (mIsDizzy)
	{
		switch (mStep)
		{
		case 0:
			CreateDizzy();
			mStep++;
			break;
		case 1:
			break;
		}
	}
}