#include "CBeamEffect.h"
#include "CBeam.h"
#include "Maths.h"

// �r�[���̊Ԋu����
#define THROW_INTERVAL 0.6f
// �r�[���̕����̃u����
#define FLAME_DIR_RAND 0.02f
// �r�[���̈ړ����x
#define FLAME_MOVE_SPEED 25.0f
// �r�[���̐F
#define FLAME_COLOR CColor(1.0f, 1.0f, 0.0f)

// �R���X�g���N�^
CBeamEffect::CBeamEffect(CObjectBase* owner, const CMatrix* attach,
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
CBeamEffect::~CBeamEffect()
{
	
}

// �r�[�����J�n
void CBeamEffect::Start()
{
	mIsThrowing = true;
	mElapsedTime = 0.0f;
}

// �r�[�����~
void CBeamEffect::Stop()
{
	mIsThrowing = false;
}

// �r�[�������Ă��邩�ǂ���
bool CBeamEffect::IsThrowing() const
{
	return mIsThrowing;
}

// �r�[�����̃I�t�Z�b�g�ʒu��ݒ�
void CBeamEffect::SetThrowOffsetPos(const CVector& pos)
{
	mThrowOffsetPos = pos;
}

// �r�[�����̃I�t�Z�b�g��]�l��ݒ�
void CBeamEffect::SetThrowOffsetRot(const CMatrix& rot)
{
	mThrowOffsetRot = rot;
}

// �r�[���̈ʒu���擾
CVector CBeamEffect::GetThrowPos() const
{
	// �A�^�b�`����s�񂪐ݒ肳��Ă���ꍇ�́A�s��̍��W��Ԃ�
	if (mpAttachMtx != nullptr)
	{
		CVector pos = mpAttachMtx->Position();
		pos += mpAttachMtx->VectorX() * mThrowOffsetPos.X()
			+ mpAttachMtx->VectorY() * mThrowOffsetPos.Y()
			+ mpAttachMtx->VectorZ() * mThrowOffsetPos.Z();
		return pos;
	}
	// �����傪�ݒ肳��Ă���ꍇ�́A������̍��W��Ԃ�
	else if (mpOwner != nullptr)
	{
		return mpOwner->Position() + (mpOwner->Rotation() * mThrowOffsetPos);
	}

	return CVector::zero;
}

// �r�[���̕������擾
CVector CBeamEffect::GetThrowDir() const
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

// �r�[�����쐬
void CBeamEffect::CreateSlash()
{
	// �r�[�����쐬
	CBeam* beam = new CBeam(ETag::eBeam);

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
	beam->Setup(pos, dir, FLAME_MOVE_SPEED);

	//slash->Rotation(40.0f, 0.0f, 0.0f);
	// �r�[���̃J���[��ݒ�
	beam->SetColor(FLAME_COLOR);
	// ���Z�u�����h�ɂ��āA�r�[�����������Ă���悤�Ɍ�����
	beam->SetBlendType(EBlend::eAdd);
}

// �X�V
void CBeamEffect::Update()
{
	// �r�[���𔭎˂��Ă�����
	if (mIsThrowing)
	{
		// �o�ߎ��Ԃɉ����āA�r�[�����쐬
		if (mElapsedTime >= THROW_INTERVAL)
		{
			CreateSlash();
			mElapsedTime -= THROW_INTERVAL;
		}
		mElapsedTime += Time::DeltaTime();
	}
}