#include "CHit.h"
#include "CCharaBase.h"
#include "Maths.h"
#include "Easing.h"

#define SCALE_TIME  0.01f  // �X�P�[���l���ő�l�ɂȂ�܂ł̎���
#define ANIM_TIME    0.2f  // �I���҂����̃A�j���[�V�����̎���
#define ANIM_SCALE  15.0f  // �I���҂����̃A�j���[�V�����Ŋg�k����X�P�[���l
#define FADEOUT_TIME 0.0f  // �I�����̃t�F�[�h�A�E�g����

// �R���X�g���N�^
CHit::CHit(float size,float height)
	: CBillBoardImage("Effect/hitEffect.png", ETag::eHit, ETaskPauseType::eGame)
	, mState(EState::Idle)
	, mElapsedTime(0.0f)
	, mBaseScale(1.0f)
	, mSize(size)
	, mHeight(height)
{
}

// �f�X�g���N�^
CHit::~CHit()
{
	Kill();
}

// ��Ԃ̐؂�ւ�
void CHit::ChangeState(EState state)
{
	if (state == mState)return;
	mState = state;
}

// ������̐ݒ�
void CHit::SetOwner(CCharaBase* owner)
{
	CBillBoardImage::SetOwner(owner);
	mBaseScale = owner->Scale().X();
}

// �q�b�g�G�t�F�N�g�J�n
void CHit::StartHitEffect()
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

// �q�b�g�G�t�F�N�g�I��
void CHit::EndHitEffect()
{
	if (mState != EState::Idle)
	{
		mElapsedTime = 0.0f;
		ChangeState(EState::End);
	}
}

// ���g�̃x�[�X�X�P�[���l���Z�o
float CHit::CalcScale() const
{
	return mOwner->Scale().X() / mBaseScale;
}

// �ҋ@��
void CHit::UpdateIdle()
{
}

// �J�n
void CHit::UpdateStart()
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
void CHit::UpdateWait()
{
	float scale = CalcScale();

	// �T�C���J�[�u�ŃX�P�[���l�̑����l���Z�o
	float percent = mElapsedTime / ANIM_TIME;
	float alpha = sinf(M_PI * 0.5f * percent);
	scale += mSize * alpha;
	Scale(scale, scale, scale);

	mElapsedTime += Time::DeltaTime();
	if (mElapsedTime >= ANIM_TIME)
	{
		//mElapsedTime -= ANIM_TIME;
		if (mState != EState::Idle)
		{
			mElapsedTime = 0.0f;
			ChangeState(EState::End);
		}
	}
}

// �I��
void CHit::UpdateEnd()
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
void CHit::Update()
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
	case EState::End:
		UpdateEnd();
		break;
	}

	// ���N���X�̍X�V����
	CBillBoardImage::Update();

	// �q�b�g�G�t�F�N�g�̒��S���W
	float scale = CalcScale();
	CVector center = mOwner->Position();
	CVector pos = CVector::zero;
	pos.Y(mHeight);
	Position(center + pos * scale);
}