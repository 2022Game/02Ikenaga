#include "CHomingBallEffect.h"
#include "CHomingBall.h"
#include "Maths.h"
#include "CCharaBase.h"

// �z�[�~���O�{�[���̔��˂̊Ԋu����
#define THROW_INTERVAL 0.4f
// �z�[�~���O�{�[���̔��˂̕����̃u����
#define DIR_RAND 0.01f
// �z�[�~���O�{�[���̐F
#define COLOR CColor(1.0f, 1.0f, 0.0f)

// �R���X�g���N�^
CHomingBallEffect::CHomingBallEffect(CObjectBase* owner, const CMatrix* attach,
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
CHomingBallEffect::~CHomingBallEffect()
{
}

// �z�[�~���O�{�[���̔��˂��J�n
void CHomingBallEffect::Start()
{
	mIsThrowing = true;
	mElapsedTime = 0.0f;
}

// �z�[�~���O�{�[���̔��˂��~
void CHomingBallEffect::Stop()
{
	mIsThrowing = false;
}

// �z�[�~���O�{�[�������Ă��邩�ǂ���
bool CHomingBallEffect::IsThrowing() const
{
	return mIsThrowing;
}

// �z�[�~���O�{�[���̔��ˎ��̃I�t�Z�b�g�ʒu��ݒ�
void CHomingBallEffect::SetThrowOffsetPos(const CVector& pos)
{
	mThrowOffsetPos = pos;
}

// �z�[�~���O�{�[���̔��ˎ��̃I�t�Z�b�g��]�l��ݒ�
void CHomingBallEffect::SetThrowOffsetRot(const CMatrix& rot)
{
	mThrowOffsetRot = rot;
}

// �z�[�~���O�{�[���̔��˂̈ʒu���擾
CVector CHomingBallEffect::GetThrowPos() const
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

// �z�[�~���O�{�[���̔��˂̕������擾
CVector CHomingBallEffect::GetThrowDir() const
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

// �z�[�~���O�{�[�����쐬
void CHomingBallEffect::CreateLightningBall()
{
	// �z�[�~���O�{�[�����쐬
	CHomingBall* homingBall = new CHomingBall(ETag::eHomingBall);
	CCharaBase* owner = dynamic_cast<CCharaBase*>(mpOwner);
	if (owner != nullptr)
	{
		homingBall->SetOwner(owner);
	}

	// �ʒu���擾
	CVector pos = GetThrowPos();// + CVector(0.0f, 10.0f, 0.0f);
	// �������擾
	CVector dir = GetThrowDir();// + CVector(0.0f, -1.0f, 0.0f);
	// �����������_���Ńu����
	dir.X(dir.X() + Math::Rand(-DIR_RAND, DIR_RAND));
	dir.Y(dir.Y() + Math::Rand(-DIR_RAND, DIR_RAND));
	dir.Z(dir.Z() + Math::Rand(-DIR_RAND, DIR_RAND));
	dir.Normalize();
	// �ʒu�A�����A�ړ����x��ݒ�
	homingBall->Setup(pos, dir, 50.0f);

	// �z�[�~���O�{�[���̃J���[��ݒ�
	homingBall->SetColor(COLOR);
	// ���Z�u�����h�ɂ��āA�z�[�~���O�{�[�����������Ă���悤�Ɍ�����
	homingBall->SetBlendType(EBlend::eAdd);
}

// �X�V
void CHomingBallEffect::Update()
{
	// �����𔭎˂��Ă�����
	if (mIsThrowing)
	{
		// �o�ߎ��Ԃɉ����āA�������쐬
		if (mElapsedTime >= THROW_INTERVAL)
		{
			CreateLightningBall();
			mElapsedTime -= THROW_INTERVAL;
		}
		mElapsedTime += Time::DeltaTime();
	}
}