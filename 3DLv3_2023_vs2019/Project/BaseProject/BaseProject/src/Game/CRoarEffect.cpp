#include "CRoarEffect.h"
#include "CRoar.h"
#include "CInput.h"
#include "Maths.h"

CRoarEffect* CRoarEffect::spInstance = nullptr;

// �Y���т̊Ԋu����
#define THROW_INTERVAL 0.6f
// �Y���т̕����̃u����
#define FLAME_DIR_RAND 0.02f
// �Y���т̈ړ����x
#define FLAME_MOVE_SPEED 25.0f
// �Y���т̐F
#define FLAME_COLOR CColor(1.0f, 1.0f, 1.0f)

// �R���X�g���N�^
CRoarEffect::CRoarEffect(CObjectBase* owner, const CMatrix* attach,
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
CRoarEffect::~CRoarEffect()
{
	spInstance = nullptr;
}

// �Y���т��J�n
void CRoarEffect::Start()
{
	mIsThrowing = true;
	mElapsedTime = 0.0f;
}

// �Y���т��~
void CRoarEffect::Stop()
{
	mIsThrowing = false;
}

// �Y���т����Ă��邩�ǂ���
bool CRoarEffect::IsThrowing() const
{
	return mIsThrowing;
}

// �Y���ю��̃I�t�Z�b�g�ʒu��ݒ�
void CRoarEffect::SetThrowOffsetPos(const CVector& pos)
{
	mThrowOffsetPos = pos;
}

// �Y���ю��̃I�t�Z�b�g��]�l��ݒ�
void CRoarEffect::SetThrowOffsetRot(const CMatrix& rot)
{
	mThrowOffsetRot = rot;
}

// �Y���т̈ʒu���擾
CVector CRoarEffect::GetThrowPos() const
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

// �Y���т̕������擾
CVector CRoarEffect::GetThrowDir() const
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

// �Y���т��쐬
void CRoarEffect::CreateSlash()
{
	// �Y���т��쐬
	CRoar* roar = new CRoar(ETag::eNone);

	// �ʒu���擾
	CVector pos = GetThrowPos();// + CVector(0.0f, 10.0f, 0.0f);
	// �������擾
	CVector dir = GetThrowDir();// + CVector(0.0f, -1.0f, 0.0f);
	// �����������_���Ńu����
	//dir.X(dir.X() + Math::Rand(-FLAME_DIR_RAND, FLAME_DIR_RAND));
	//dir.Y(dir.Y() + Math::Rand(-FLAME_DIR_RAND, FLAME_DIR_RAND));
	//dir.Z(dir.Z() + Math::Rand(-FLAME_DIR_RAND, FLAME_DIR_RAND));
	dir.Normalize();
	// �ʒu�A�����A�ړ����x��ݒ�
	roar->Setup(pos, dir, FLAME_MOVE_SPEED);

	//slash->Rotation(40.0f, 0.0f, 0.0f);
	// �Y���т̃J���[��ݒ�
	roar->SetColor(FLAME_COLOR);
	// ���Z�u�����h�ɂ��āA�Y���т��������Ă���悤�Ɍ�����
	roar->SetBlendType(EBlend::eAdd);
}

// �X�V
void CRoarEffect::Update()
{
	// �Y���т𔭎˂��Ă�����
	if (mIsThrowing)
	{
		// �o�ߎ��Ԃɉ����āA�Y���т��쐬
		if (mElapsedTime >= THROW_INTERVAL)
		{
			CreateSlash();
			mElapsedTime -= THROW_INTERVAL;
		}
		mElapsedTime += Time::DeltaTime();
	}
}