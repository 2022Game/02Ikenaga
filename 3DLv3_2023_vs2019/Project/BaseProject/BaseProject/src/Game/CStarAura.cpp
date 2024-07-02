#include "CStarAura.h"
#include "CCharaBase.h"
#include "Easing.h"
#include "CHealCircle.h"
#include "Maths.h"

// ���̃X�P�[���l�̍ő�l
#define SCALE 4.0f
// ���̃X�P�[���l���ő�l�ɂȂ�܂ł̎���
#define SCALE_ANIM_TIME 0.2f

// �R���X�g���N�^
CStarAura::CStarAura(const CVector& pos, const CVector& dir, float speed, float dist)
	: CBillBoardImage("Effect/star.png", ETag::eAura, ETaskPauseType::eGame)
	, mMoveDir(CVector::zero)
	, mElapsedTime(0.0f)
	, mMovedDist(0.0f)
	, mKillMoveDist(dist)
{
	Position(pos);
	mMoveSpeed = dir.Normalized() * speed;
}

// �f�X�g���N�^
CStarAura::~CStarAura()
{
}

// �J���[��ݒ�
void CStarAura::SetColor(const CColor& color)
{
	mMaterial.Diffuse()[0] = color.R();
	mMaterial.Diffuse()[1] = color.G();
	mMaterial.Diffuse()[2] = color.B();
	mMaterial.Diffuse()[3] = color.A();
	CObjectBase::SetColor(color);
}

// �u�����h�^�C�v��ݒ�
void CStarAura::SetBlendType(EBlend type)
{
	mMaterial.SetBlendType(type);
}

// �X�V
void CStarAura::Update()
{
	// ���N���X�̍X�V����
	CBillBoardImage::Update();

	// �X�P�[���ύX���Ԃ��o�߂��Ă��Ȃ�
	if (mElapsedTime < SCALE_ANIM_TIME)
	{
		// �o�ߎ��Ԃɍ��킹�āA���X�ɐ���傫������
		float per = mElapsedTime / SCALE_ANIM_TIME;
		if (per < 1.0f)
		{
			float scale = Easing::QuadOut(per, 1.0f, 1.0f, 1.0f);
			Scale(CVector::one * scale * SCALE);
		}
		else
		{
			Scale(CVector::one * SCALE);
		}

		mElapsedTime += Time::DeltaTime();
	}
	// �ړ����Ԃ��o�߂�����A�폜����
	else
	{
		Scale(CVector::one * SCALE);
	}

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