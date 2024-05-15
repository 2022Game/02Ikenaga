#include "CLightningBallEffect.h"
#include "CLightningBall.h"
#include "Maths.h"

CLightningBallEffect* CLightningBallEffect::spInstance = nullptr;

// �������˂̊Ԋu����
#define THROW_INTERVAL 0.4f
// �������˂̕����̃u����
#define FLAME_DIR_RAND 0.02f
// �������˂̈ړ����x
#define FLAME_MOVE_SPEED 100.0f
// �����̐F
#define FLAME_COLOR CColor(1.0f, 1.0f, 0.0f)

// �R���X�g���N�^
CLightningBallEffect::CLightningBallEffect(CObjectBase* owner, const CMatrix* attach,
	const CVector& offsetPos, const CMatrix& offsetRot)
	: mpOwner(owner)
	, mpAttachMtx(attach)
	, mThrowOffsetPos(offsetPos)
	, mThrowOffsetRot(offsetRot)
	, mElapsedTime(0.0f)
	, mIsThrowing(false)
{
	spInstance = this;
}

// �f�X�g���N�^
CLightningBallEffect::~CLightningBallEffect()
{
	spInstance = nullptr;
}

// �������˂��J�n
void CLightningBallEffect::Start()
{
	mIsThrowing = true;
	mElapsedTime = 0.0f;
}

// �������˂��~
void CLightningBallEffect::Stop()
{
	mIsThrowing = false;
}

// ���������Ă��邩�ǂ���
bool CLightningBallEffect::IsThrowing() const
{
	return mIsThrowing;
}

// �������ˎ��̃I�t�Z�b�g�ʒu��ݒ�
void CLightningBallEffect::SetThrowOffsetPos(const CVector& pos)
{
	mThrowOffsetPos = pos;
}

// �������ˎ��̃I�t�Z�b�g��]�l��ݒ�
void CLightningBallEffect::SetThrowOffsetRot(const CMatrix& rot)
{
	mThrowOffsetRot = rot;
}

// �������˂̈ʒu���擾
CVector CLightningBallEffect::GetThrowPos() const
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

// �������˂̕������擾
CVector CLightningBallEffect::GetThrowDir() const
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

// �������쐬
void CLightningBallEffect::CreateLightningBall()
{
	// �������쐬
	CLightningBall* lightningBall = new CLightningBall(ETag::eLightningBall);

	// �ʒu���擾
	CVector pos = GetThrowPos();// + CVector(0.0f, 10.0f, 0.0f);
	// �������擾
	CVector dir = GetThrowDir();// + CVector(0.0f, -1.0f, 0.0f);
	// �����������_���Ńu����
	dir.X(dir.X() + Math::Rand(-FLAME_DIR_RAND, FLAME_DIR_RAND));
	dir.Y(dir.Y() + Math::Rand(-FLAME_DIR_RAND, FLAME_DIR_RAND));
	dir.Z(dir.Z() + Math::Rand(-FLAME_DIR_RAND, FLAME_DIR_RAND));
	dir.Normalize();
	// �ʒu�A�����A�ړ����x��ݒ�
	lightningBall->Setup(pos, dir, FLAME_MOVE_SPEED);

	// �����̃J���[��ݒ�
	lightningBall->SetColor(FLAME_COLOR);
	// ���Z�u�����h�ɂ��āA�������������Ă���悤�Ɍ�����
	lightningBall->SetBlendType(EBlend::eAdd);
}

// �X�V
void CLightningBallEffect::Update()
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