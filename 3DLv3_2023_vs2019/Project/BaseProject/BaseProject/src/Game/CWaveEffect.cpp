#include "CWaveEffect.h"
#include "CWave.h"
#include "CInput.h"
#include "Maths.h"

CWaveEffect* CWaveEffect::spInstance = nullptr;

// �g���̊Ԋu����
#define THROW_INTERVAL 0.2f
// �g���̕����̃u����
#define FLAME_DIR_RAND 0.02f
// �g���̈ړ����x
#define FLAME_MOVE_SPEED 25.0f
// �g���̐F
#define FLAME_COLOR CColor(1.0f, 0.0f, 1.0f)

// �R���X�g���N�^
CWaveEffect::CWaveEffect(CObjectBase* owner, const CMatrix* attach,
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
CWaveEffect::~CWaveEffect()
{
	spInstance = nullptr;
}

// �g�����J�n
void CWaveEffect::Start()
{
	mIsThrowing = true;
	mElapsedTime = 0.0f;
}

// �g�����~
void CWaveEffect::Stop()
{
	mIsThrowing = false;
}

// �g�������Ă��邩�ǂ���
bool CWaveEffect::IsThrowing() const
{
	return mIsThrowing;
}

// �g�����̃I�t�Z�b�g�ʒu��ݒ�
void CWaveEffect::SetThrowOffsetPos(const CVector& pos)
{
	mThrowOffsetPos = pos;
}

// �g�����̃I�t�Z�b�g��]�l��ݒ�
void CWaveEffect::SetThrowOffsetRot(const CMatrix& rot)
{
	mThrowOffsetRot = rot;
}

// �g���̈ʒu���擾
CVector CWaveEffect::GetThrowPos() const
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

// �g���̕������擾
CVector CWaveEffect::GetThrowDir() const
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

// �g�����쐬
void CWaveEffect::CreateSlash()
{
	// �Y���т��쐬
	CWave* wave = new CWave(ETag::eWave);

	// �ʒu���擾
	CVector pos = GetThrowPos();// + CVector(0.0f, 10.0f, 0.0f);
	// �������擾
	CVector dir = GetThrowDir();// + CVector(0.0f, -1.0f, 0.0f);
	// �����������_���Ńu����
	dir.X(dir.X() + Math::Rand(-FLAME_DIR_RAND, FLAME_DIR_RAND));
	dir.Y(dir.Y() + Math::Rand(-FLAME_DIR_RAND, FLAME_DIR_RAND));
	dir.Z(dir.Z() + Math::Rand(-FLAME_DIR_RAND, FLAME_DIR_RAND));
	//dir.Normalize();
	// �ʒu�A�����A�ړ����x��ݒ�
	wave->Setup(pos, dir, FLAME_MOVE_SPEED);

	// �g���̃J���[��ݒ�
	wave->SetColor(FLAME_COLOR);
	// ���Z�u�����h�ɂ��āA�g�����������Ă���悤�Ɍ�����
	wave->SetBlendType(EBlend::eAdd);
}

// �X�V
void CWaveEffect::Update()
{
	// �g���𔭎˂��Ă�����
	if (mIsThrowing)
	{
		// �o�ߎ��Ԃɉ����āA�g�����쐬
		if (mElapsedTime >= THROW_INTERVAL)
		{
			CreateSlash();
			mElapsedTime -= THROW_INTERVAL;
		}
		mElapsedTime += Time::DeltaTime();
	}
}