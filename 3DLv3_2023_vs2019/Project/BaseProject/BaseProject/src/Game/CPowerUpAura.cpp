#include "CPowerUpAura.h"
#include "CCharaBase.h"
#include "CPlayer.h"
#include "Maths.h"

// �R���X�g���N�^
CPowerUpAura::CPowerUpAura(float angle, float width, const CVector& dire, float speed, float dist)
	: CBillBoardImage("Effect/Buff.png", ETag::eAura, ETaskPauseType::eGame)
	, mPowerUp(false)
	, mElapsedPowerUpTime(0.0f)
	, mAngle(angle)
	, mDistance(width)
	, mMoveDir(CVector::zero)
	, mMovedDist(0.0f)
	, mKillMoveDist(dist)
	, mBaseScale(1.0f)
{
	mMoveSpeed = dire.Normalized() * speed;
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

// �X�V
void CPowerUpAura::Update()
{
	mPowerUp = mOwner->IsPowerUp();
	mElapsedPowerUpTime = mOwner->GetElapsedPowerUpTime();

	// ���N���X�̍X�V����
	CBillBoardImage::Update();

	if (mPowerUp == true)
	{

		// ������̃x�[�X�X�P�[���l����A
		// ���݂̊g�嗦�����߂āA�I�[���ɂ����f
		float scale = mOwner->Scale().X() / mBaseScale;
		float size = 7.0f;
		Scale(scale + size, scale + size, scale + size);

		// ��]����I�[���̒��S���W
		CVector center = mOwner->Position();
		// ���S���W���猻�݂̉�]�ō��W�����߂�(�~�^��)
		CVector pos = CVector::zero;
		center.X(center.X() + Math::Rand(-20.0f, 20.0f));
		center.Y(center.Y() + Math::Rand(5.0f, 5.0f));
		center.Z(center.Z() + Math::Rand(-20.0f, 20.0f));
		pos.Y(mMovedDist);
		Position(center + pos * scale);

		// �ړ��x�N�g�������߂�
		CVector move = mMoveSpeed * Time::DeltaTime();

		float moveDist = move.Length();
		if (mMovedDist + moveDist >= mKillMoveDist)
		{
			moveDist = mKillMoveDist - mMovedDist;
			move = move.Normalized() * moveDist;
		}

		if (mMovedDist < mKillMoveDist)
		{
			Position(Position() + move);
		}

		mMovedDist += moveDist;
		if (mMovedDist >= mKillMoveDist)
		{
			Kill();
		}
	}
	else
	{
		SetShow(false);
	}
}