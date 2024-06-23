#include "CDrainEffect.h"
#include "CDrain.h"
#include "Maths.h"
#include "CCharaBase.h"

// �h���C���̔��˂̊Ԋu����
#define THROW_INTERVAL 0.8f
// �h���C���̔��˂̕����̃u����
#define DIR_RAND 0.01f
// �h���C���̐F
#define COLOR CColor(1.0f, 0.0f, 0.5f)

// �R���X�g���N�^
CDrainEffect::CDrainEffect(CObjectBase* owner, const CMatrix* attach,
	const CVector& offsetPos, const CMatrix& offsetRot)
	: mpOwner(owner)
	, mpAttachMtx(attach)
	, mThrowOffsetPos(offsetPos)
	, mThrowOffsetRot(offsetRot)
	, mElapsedTime(0.0f)
	, mIsThrowing(false)
{
}

// �f�X�g���N�^
CDrainEffect::~CDrainEffect()
{
}

// �h���C���̔��˂��J�n
void CDrainEffect::Start()
{
	mIsThrowing = true;
	mElapsedTime = 0.0f;
}

// �h���C���̔��˂��~
void CDrainEffect::Stop()
{
	mIsThrowing = false;
}

// �h���C�������Ă��邩�ǂ���
bool CDrainEffect::IsThrowing() const
{
	return mIsThrowing;
}

// �h���C���̔��ˎ��̃I�t�Z�b�g�ʒu��ݒ�
void CDrainEffect::SetThrowOffsetPos(const CVector& pos)
{
	mThrowOffsetPos = pos;
}

// �h���C���̔��ˎ��̃I�t�Z�b�g��]�l��ݒ�
void CDrainEffect::SetThrowOffsetRot(const CMatrix& rot)
{
	mThrowOffsetRot = rot;
}

// �h���C���̔��˂̈ʒu���擾
CVector CDrainEffect::GetThrowPos() const
{
	// �A�^�b�`����s�񂪐ݒ肳��Ă���ꍇ�́A�s��̍��W��Ԃ�
	if (mpAttachMtx != nullptr)
	{
		CVector pos = mpAttachMtx->Position();
		pos += mpAttachMtx->VectorX().Normalized() * mThrowOffsetPos.X()
			+ mpAttachMtx->VectorY().Normalized() * mThrowOffsetPos.Y()
			+ mpAttachMtx->VectorZ().Normalized() * mThrowOffsetPos.Z();
		return pos;
	}
	// �����傪�ݒ肳��Ă���ꍇ�́A������̍��W��Ԃ�
	else if (mpOwner != nullptr)
	{
		return mpOwner->Position() + (mpOwner->Rotation() * mThrowOffsetPos);
	}

	return CVector::zero;
}

// �h���C���̔��˂̕������擾
CVector CDrainEffect::GetThrowDir() const
{
	// �A�^�b�`����s�񂪐ݒ肳��Ă���ꍇ�́A�s��̐��ʕ����x�N�g����Ԃ�
	if (mpAttachMtx != nullptr)
	{
		return mThrowOffsetRot * mpAttachMtx->VectorZ();
	}
	// �����傪�ݒ肳��Ă���ꍇ�́A������̐��ʕ����x�N�g����Ԃ�
	else if (mpOwner != nullptr)
	{
		return mThrowOffsetRot * mpOwner->VectorZ();
	}

	return CVector::forward;
}

// �h���C�����쐬
void CDrainEffect::CreateDrain()
{
	// �h���C�����쐬
	CDrain* drain = new CDrain(ETag::eDrain);
	CCharaBase* owner = dynamic_cast<CCharaBase*>(mpOwner);
	if (owner != nullptr)
	{
		drain->SetOwner(owner);
	}

	// �ʒu���擾
	CVector pos = GetThrowPos();
	// �������擾
	CVector dir = GetThrowDir();
	// �ʒu�A�����A�ړ����x��ݒ�
	drain->Setup(pos, dir, 50.0f);

	// �h���C���̃J���[��ݒ�
	drain->SetColor(COLOR);
	// ���Z�u�����h�ɂ��āA�h���C�����������Ă���悤�Ɍ�����
	drain->SetBlendType(EBlend::eAdd);
}

// �X�V
void CDrainEffect::Update()
{
	// �h���C���𔭎˂��Ă�����
	if (mIsThrowing)
	{
		// �o�ߎ��Ԃɉ����āA�h���C�����쐬
		if (mElapsedTime >= THROW_INTERVAL)
		{
			CreateDrain();
			mElapsedTime -= THROW_INTERVAL;
		}
		mElapsedTime += Time::DeltaTime();
	}
}