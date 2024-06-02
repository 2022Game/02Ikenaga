#include "CImpactEffect.h"
#include "CImpact.h"
#include "Maths.h"
#include "CCharaBase.h"

// �Ռ��̊Ԋu����
#define THROW_INTERVAL 0.4f
// �Ռ��̕����̃u����
#define FLAME_DIR_RAND 0.02f
// �Ռ��̈ړ����x
#define FLAME_MOVE_SPEED 70.0f
// �Ռ��̐F
//#define FLAME_COLOR CColor(1.0f, 1.0f, 0.0f)

// �R���X�g���N�^
CImpactEffect::CImpactEffect(CObjectBase* owner, const CMatrix* attach,
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
CImpactEffect::~CImpactEffect()
{
}

// �Ռ����J�n
void CImpactEffect::Start()
{
	mIsThrowing = true;
	mElapsedTime = 0.0f;
}

// �Ռ����~
void CImpactEffect::Stop()
{
	mIsThrowing = false;
}

// �Ռ������Ă��邩�ǂ���
bool CImpactEffect::IsThrowing() const
{
	return mIsThrowing;
}

// �Ռ����̃I�t�Z�b�g�ʒu��ݒ�
void CImpactEffect::SetThrowOffsetPos(const CVector& pos)
{
	mThrowOffsetPos = pos;
}

// �Ռ����̃I�t�Z�b�g��]�l��ݒ�
void CImpactEffect::SetThrowOffsetRot(const CMatrix& rot)
{
	mThrowOffsetRot = rot;
}

// �Ռ��̈ʒu���擾
CVector CImpactEffect::GetThrowPos() const
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

// �Ռ��̕������擾
CVector CImpactEffect::GetThrowDir() const
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

// �Ռ����쐬
void CImpactEffect::CreateImpact()
{
	// �Ռ����쐬
	CImpact* impact = new CImpact(ETag::eImpact);
	CCharaBase* owner = dynamic_cast<CCharaBase*>(mpOwner);
	if (owner != nullptr)
	{
		impact->SetOwner(owner);
	}

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
	impact->Setup(pos, dir, FLAME_MOVE_SPEED);

	// ���Z�u�����h�ɂ��āA�Ռ����������Ă���悤�Ɍ�����
	impact->SetBlendType(EBlend::eAdd);

	// �쐬�����Ռ��̃G�t�F�N�g�����X�g�ɒǉ�
	mImpact.push_back(impact);
}

// �X�V
void CImpactEffect::Update()
{
	// �Ռ��𔭎˂��Ă�����
	if (mIsThrowing)
	{
		// �o�ߎ��Ԃɉ����āA�Ռ����쐬
		if (mElapsedTime >= THROW_INTERVAL)
		{
			CreateImpact();
			mElapsedTime -= THROW_INTERVAL;
		}
		mElapsedTime += Time::DeltaTime();
	}
}