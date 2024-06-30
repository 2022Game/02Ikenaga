#include "CBuffCircle.h"
#include "CCollisionManager.h"
#include "CPlayer.h"
#include "Maths.h"
#include "Easing.h"

#define SCALE_TIME   1.0f  // �X�P�[���l���ő�l�ɂȂ�܂ł̎���
#define ANIM_TIME    3.0f  // �I���҂����̃A�j���[�V�����̎���
#define ANIM_SCALE  0.25f  // �I���҂����̃A�j���[�V�����Ŋg�k����X�P�[���l
#define FADEOUT_TIME 2.0f  // �I�����̃t�F�[�h�A�E�g����

// �R���X�g���N
CBuffCircle::CBuffCircle()
	: mState(EState::Idle)
	, mElapsedTime(0.0f)
	, mBaseScale(1.0f)
{
	mpBuffCircle = CResourceManager::Get<CModel>("BuffCircle");
	mpBuffCircle->SetupEffectSettings();

	SetAlpha(0.0f);
	SetEnable(false);
	SetShow(false);
}

// �f�X�g���N�^
CBuffCircle::~CBuffCircle()
{
}

// �������ݒ�
void CBuffCircle::SetOwner(CCharaBase* owner)
{
	CWeapon::SetOwner(owner);
	mBaseScale = owner->Scale().X();
}

// ��Ԃ̐؂�ւ�
void CBuffCircle::ChangeState(EState state)
{
	if (state == mState)return;
	mState = state;
}

// �o�t�T�[�N���J�n
void CBuffCircle::StartCircle()
{
	if (mState == EState::Idle)
	{
		SetEnable(true);
		SetShow(true);
		ChangeState(EState::Start);
		Scale(0.0f, 0.0f, 0.0f);
		SetAlpha(1.0f);
	}
}

// �o�t�T�[�N���I��
void CBuffCircle::EndCircle()
{
	if(mState != EState::Idle)
	{
		mElapsedTime = 0.0f;
		ChangeState(EState::End);
	}
}

// ���g�̃x�[�X�X�P�[���l���Z�o
float CBuffCircle::CalcScale() const
{
	return mOwner->Scale().X() / mBaseScale;
}

// �ҋ@��
void CBuffCircle::UpdateIdle()
{
}

// �J�n
void CBuffCircle::UpdateStart()
{
	float scale = CalcScale();
	
	// �o�ߎ��Ԃɍ��킹�đ傫������
	if (mElapsedTime < SCALE_TIME)
	{
		float percent = mElapsedTime / SCALE_TIME;
		scale *= percent;
		Scale(scale, scale, scale);

		mElapsedTime += Time::DeltaTime();
	}
	else
	{
		Scale(scale, scale, scale);
		mElapsedTime -= SCALE_TIME;
		ChangeState(EState::Wait);
	}
}

// �I���҂�
void CBuffCircle::UpdateWait()
{
	float scale = CalcScale();

	// �T�C���J�[�u�ŃX�P�[���l�̑����l���Z�o
	float percent = mElapsedTime / ANIM_TIME;
	float alpha = sinf(M_PI * 2.0f * percent);
	scale += ANIM_SCALE * alpha;
	Scale(scale, scale, scale);

	mElapsedTime += Time::DeltaTime();
	if (mElapsedTime >= ANIM_TIME)
	{
		mElapsedTime -= ANIM_TIME;
	}
}

// �I��
void CBuffCircle::UpdateEnd()
{
	// �t�F�[�h�A�E�g��
	if (mElapsedTime < FADEOUT_TIME)
	{
		// �o�ߎ��Ԃ���A���t�@�l�����߂�
		float alpha = 1.0f - mElapsedTime / FADEOUT_TIME;
		SetAlpha(alpha);

		mElapsedTime += Time::DeltaTime();
	}
	// �t�F�[�h�A�E�g�I��
	else
	{
		SetAlpha(0.0f);
		ChangeState(EState::Idle);
		mElapsedTime = 0.0f;
		SetEnable(false);
		SetShow(false);
	}
}

// �X�V
void CBuffCircle::Update()
{
	switch (mState)
	{
	case EState::Idle:
		UpdateIdle();
		break;
	case EState::Start:
		UpdateStart();
		break;
	case EState::Wait:
		UpdateWait();
		break;
	case EState::End :
		UpdateEnd();
		break;
	}

	// �T�[�N���̒��S���W
	float scale = CalcScale();
	CVector center = mOwner->Position();
	CVector pos = CVector::zero;
	center.Y(0.0f);
	pos.Y(0.4f * scale);
	Position(center + pos);
}

// �`��
void CBuffCircle::Render()
{
	if (mpAttachMtx == nullptr)
	{
		CColor c = mColor;
		c.R(c.R() * c.A());
		c.G(c.G() * c.A());
		c.B(c.B() * c.A());
		mpBuffCircle->SetColor(c);
		mpBuffCircle->Render(Matrix());
	}
	else
	{
		CMatrix sm;
		sm.Scale(100.0f, 100.0f, 100.0f);
		mpBuffCircle->Render(sm * (*mpAttachMtx));
	}
}