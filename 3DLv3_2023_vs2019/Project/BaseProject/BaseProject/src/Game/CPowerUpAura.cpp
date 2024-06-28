#include "CPowerUpAura.h"
#include "CCharaBase.h"
#include "CPlayer.h"
#include "Maths.h"

// �R���X�g���N�^
CPowerUpAura::CPowerUpAura(const CVector& pos, const CVector& dir, float speed, float dist)
	: CBillBoardImage("Effect/Ball.png", ETag::eAura, ETaskPauseType::eGame)
	, mMoveDir(CVector::zero)
	, mMovedDist(0.0f)
	, mKillMoveDist(dist)
	, mElapsedTime(0.0f)
	, mBaseScale(1.0f)
{
	Position(pos);
	mMoveSpeed = dir.Normalized() * speed;
}

// �f�X�g���N�^
CPowerUpAura::~CPowerUpAura()
{
}

// �������ݒ�
void CPowerUpAura::SetOwner(CCharaBase* owner)
{
	CBillBoardImage::SetOwner(owner);
	mBaseScale = owner->Scale().X();
}

// �J���[��ݒ�
void CPowerUpAura::SetColor(const CColor& color)
{
	mMaterial.Diffuse()[0] = color.R();
	mMaterial.Diffuse()[1] = color.G();
	mMaterial.Diffuse()[2] = color.B();
	mMaterial.Diffuse()[3] = color.A();
	CObjectBase::SetColor(color);
}

// �u�����h�^�C�v��ݒ�
void CPowerUpAura::SetBlendType(EBlend type)
{
	mMaterial.SetBlendType(type);
}

// �o�t�I�[���J�n
void CPowerUpAura::StartAura()
{
	
}

// �o�t�I�[���I��
void CPowerUpAura::EndAura()
{
	
}

// ���g�̃x�[�X�X�P�[���l���Z�o
float CPowerUpAura::CalcScale() const
{
	return mOwner->Scale().X() / mBaseScale;
}

// �X�V
void CPowerUpAura::Update()
{
	// ���N���X�̍X�V����
	CBillBoardImage::Update();

	// �ړ��x�N�g�������߂�
	CVector move = mMoveSpeed * Time::DeltaTime();

	float moveDist = move.Length();
	if (mMovedDist + moveDist >= mKillMoveDist)
	{
		moveDist = mKillMoveDist - mMovedDist;
		move = move.Normalized() * moveDist;
	}
	move.Y(0.1f);
	Position(Position() + move);

	mMovedDist += moveDist;
	if (mMovedDist >= mKillMoveDist)
	{
		Kill();
	}
}