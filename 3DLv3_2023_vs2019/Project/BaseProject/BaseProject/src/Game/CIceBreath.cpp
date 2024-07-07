#include "CIceBreath.h"
#include "CBreath.h"
#include "Maths.h"
#include "CCharaBase.h"

// �A�C�X�u���X�̔��ˊԊu����
#define THROW_INTERVAL 0.01f
// �A�C�X�u���X�̔��˕����̃u����
#define BREATH_DIR_RAND 0.01f
// �A�C�X�u���X�̈ړ����x
#define BREATH_MOVE_SPEED 50.0f
// �A�C�X�u���X�̐F
#define BREATH_COLOR CColor(0.0f, 1.0f, 1.0f)

// �R���X�g���N�^
CIceBreath::CIceBreath(CObjectBase* owner, const CMatrix* attach,
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
CIceBreath::~CIceBreath()
{
	for (CBreath* breath : mBreath)
	{
		breath->Kill();
	}
}

// ���ˊJ�n
void CIceBreath::Start()
{
	mIsThrowing = true;
	mElapsedTime = 0.0f;
}

// ���˒�~
void CIceBreath::Stop()
{
	mIsThrowing = false;
}

// �A�C�X�u���X�𔭎˂��Ă��邩�ǂ���
bool CIceBreath::IsThrowing() const
{
	return mIsThrowing;
}

// ���ˎ��̃I�t�Z�b�g�ʒu��ݒ�
void CIceBreath::SetThrowOffsetPos(const CVector& pos)
{
	mThrowOffsetPos = pos;
}

// ���ˎ��̃I�t�Z�b�g��]�l��ݒ�
void CIceBreath::SetThrowOffsetRot(const CMatrix& rot)
{
	mThrowOffsetRot = rot;
}

// �A�C�X�u���X�̔��ˈʒu���擾
CVector CIceBreath::GetThrowPos() const
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

// �A�C�X�u���X�̔��˕������擾
CVector CIceBreath::GetThrowDir() const
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

	return mThrowOffsetRot * CVector::forward;
}

// �u���X�̃G�t�F�N�g���쐬
void CIceBreath::CreateBreath()
{
	CBreath* breath = new CBreath(ETag::eFlame);
	CCharaBase* owner = dynamic_cast<CCharaBase*>(mpOwner);
	if (owner != nullptr)
	{
		breath->SetOwner(owner);
	}

	// ���ˈʒu���擾
	CVector pos = GetThrowPos();// + CVector(0.0f, 10.0f, 0.0f);
	// ���˕������擾
	CVector dir = GetThrowDir();// + CVector(0.0f, -1.0f, 0.0f);
	// ���˕����������_���Ńu����
	dir.X(dir.X() + Math::Rand(-BREATH_DIR_RAND, BREATH_DIR_RAND));
	dir.Y(dir.Y() + Math::Rand(0.0f, 0.05f));
	dir.Z(dir.Z() + Math::Rand(-BREATH_DIR_RAND, BREATH_DIR_RAND));
	dir.Normalize();
	// ���ˈʒu�A�����A�ړ����x��ݒ�
	breath->Setup(pos, dir, BREATH_MOVE_SPEED);

	// �u���X�̃J���[��ݒ�
	breath->SetColor(BREATH_COLOR);
	// ���Z�u�����h�ɂ��āA�u���X���������Ă���悤�Ɍ�����
	breath->SetBlendType(EBlend::eAdd);

	// �쐬�����u���X�̃G�t�F�N�g�����X�g�ɒǉ�
	mBreath.push_back(breath);
}

// �X�V
void CIceBreath::Update()
{
	// �u���X�𔭎˂��Ă�����
	if (mIsThrowing)
	{
		// �o�ߎ��Ԃɉ����āA�u���X�̃G�t�F�N�g���쐬
		if (mElapsedTime >= THROW_INTERVAL)
		{
			CreateBreath();
			mElapsedTime -= THROW_INTERVAL;
		}
		mElapsedTime += Time::DeltaTime();
	}

	// �����ς݂̃u���X�̃G�t�F�N�g�̍폜����
	auto itr = mBreath.begin();
	auto end = mBreath.end();
	while (itr != end)
	{
		CBreath* breath = *itr;
		// �폜�t���O�������Ă�����A�폜
		if (breath->IsDeath())
		{
			itr = mBreath.erase(itr);
			breath->Kill();
		}
		else
		{
			itr++;
		}
	}
}