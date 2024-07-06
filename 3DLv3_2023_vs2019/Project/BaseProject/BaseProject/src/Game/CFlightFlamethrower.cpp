#include "CFlightFlamethrower.h"
#include "CFlightFlame.h"
#include "Maths.h"
#include "CCharaBase.h"

// ���̔��ˊԊu����
#define THROW_INTERVAL 0.03f
// ���̔��˕����̃u����
#define FLAME_DIR_RAND 0.01f
// ���̈ړ����x
#define FLAME_MOVE_SPEED 80.0f
// ���̐F(��)
#define FLAME_COLOR CColor(0.0f, 0.1f, 1.0f)

// �R���X�g���N�^
CFlightFlamethrower::CFlightFlamethrower(CObjectBase* owner, const CMatrix* attach,
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
CFlightFlamethrower::~CFlightFlamethrower()
{
	for (CFlightFlame* flame : mFlightFlames)
	{
		flame->Kill();
	}
}

// ���𔭎ˊJ�n
void CFlightFlamethrower::Start()
{
	mIsThrowing = true;
	mElapsedTime = 0.0f;
}

// ���𔭎˒�~
void CFlightFlamethrower::Stop()
{
	mIsThrowing = false;
}

// ���𔭎˂��Ă��邩�ǂ���
bool CFlightFlamethrower::IsThrowing() const
{
	return mIsThrowing;
}

// ���ˎ��̃I�t�Z�b�g�ʒu��ݒ�
void CFlightFlamethrower::SetThrowOffsetPos(const CVector& pos)
{
	mThrowOffsetPos = pos;
}

// ���ˎ��̃I�t�Z�b�g��]�l��ݒ�
void CFlightFlamethrower::SetThrowOffsetRot(const CMatrix& rot)
{
	mThrowOffsetRot = rot;
}

// ���̔��ˈʒu���擾
CVector CFlightFlamethrower::GetThrowPos() const
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

// ���̔��˕������擾
CVector CFlightFlamethrower::GetThrowDir() const
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

// ���̃G�t�F�N�g���쐬
void CFlightFlamethrower::CreateFlame()
{
	// ���̃G�t�F�N�g���쐬
	CFlightFlame* flame = new CFlightFlame(ETag::eFlame);
	CCharaBase* owner = dynamic_cast<CCharaBase*>(mpOwner);
	if (owner != nullptr)
	{
		flame->SetOwner(owner);
	}

	// ���ˈʒu���擾
	CVector pos = GetThrowPos();// + CVector(0.0f, 10.0f, 0.0f);
	// ���˕������擾
	CVector dir = GetThrowDir();// + CVector(0.0f, -1.0f, 0.0f);
	// ���˕����������_���Ńu����
	dir.X(dir.X() + Math::Rand(-FLAME_DIR_RAND, FLAME_DIR_RAND));
	dir.Y(dir.Y() + -0.3f);
	dir.Z(dir.Z() + Math::Rand(-FLAME_DIR_RAND, FLAME_DIR_RAND));
	dir.Normalize();
	// ���ˈʒu�A�����A�ړ����x��ݒ�
	flame->Setup(pos, dir, FLAME_MOVE_SPEED);

	// ���̃J���[��ݒ�
	flame->SetColor(FLAME_COLOR);
	// ���Z�u�����h�ɂ��āA�����������Ă���悤�Ɍ�����
	flame->SetBlendType(EBlend::eAdd);

	// �쐬�������̃G�t�F�N�g�����X�g�ɒǉ�
	mFlightFlames.push_back(flame);
}

// �X�V
void CFlightFlamethrower::Update()
{
	// ���𔭎˂��Ă�����
	if (mIsThrowing)
	{
		// �o�ߎ��Ԃɉ����āA���̃G�t�F�N�g���쐬
		if (mElapsedTime >= THROW_INTERVAL)
		{
			CreateFlame();
			mElapsedTime -= THROW_INTERVAL;
		}
		mElapsedTime += Time::DeltaTime();
	}

	// �����ς݂̉��̃G�t�F�N�g�̍폜����
	auto itr = mFlightFlames.begin();
	auto end = mFlightFlames.end();
	while (itr != end)
	{
		CFlightFlame* flame = *itr;
		// �폜�t���O�������Ă�����A�폜
		if (flame->IsDeath())
		{
			itr = mFlightFlames.erase(itr);
			flame->Kill();
		}
		else
		{
			itr++;
		}
	}
}