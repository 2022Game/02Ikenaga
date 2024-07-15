#include "CBuffAura.h"
#include "CCharaBase.h"
#include "Maths.h"
#include "CPlayer.h"

#define SCALE_TIME   0.1f  // �X�P�[���l���ő�l�ɂȂ�܂ł̎���
#define ANIM_TIME    0.1f  // �I���҂����̃A�j���[�V�����̎���
#define ANIM_SCALE   1.5f  // �I���҂����̃A�j���[�V�����Ŋg�k����X�P�[���l
#define FADEOUT_TIME 0.5f  // �I�����̃t�F�[�h�A�E�g����

// �R���X�g���N�^
CBuffAura::CBuffAura()
	: CBillBoardImage("Effect/Aura1.png", ETag::eAura, ETaskPauseType::eGame)
	, mState(EState::Idle)
	, mElapsedTime(0.0f)
	, mBaseScale(1.0f)
{
	SetAlpha(0.0f);
	SetEnable(false);
	SetShow(false);
}

// �f�X�g���N�^
CBuffAura::~CBuffAura()
{
}

// �������ݒ�
void CBuffAura::SetOwner(CCharaBase* owner)
{
	CBillBoardImage::SetOwner(owner);
	mBaseScale = owner->Scale().X();
}

// �J���[��ݒ�
void CBuffAura::SetColor(const CColor& color)
{
	mMaterial.Diffuse()[0] = color.R();
	mMaterial.Diffuse()[1] = color.G();
	mMaterial.Diffuse()[2] = color.B();
	mMaterial.Diffuse()[3] = color.A();
	CObjectBase::SetColor(color);
}

// �u�����h�^�C�v��ݒ�
void CBuffAura::SetBlendType(EBlend type)
{
	mMaterial.SetBlendType(type);
}

// ��Ԃ̐؂�ւ�
void CBuffAura::ChangeState(EState state)
{
	if (state == mState)return;
	mState = state;
}

// �o�t�I�[���J�n
void CBuffAura::StartAura()
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

// �o�t�I�[���I��
void CBuffAura::EndAura()
{
	if (mState != EState::Idle)
	{
		mElapsedTime = 0.0f;
		ChangeState(EState::End);
	}
}

// ���g�̃x�[�X�X�P�[���l���Z�o
float CBuffAura::CalcScaleXZ() const
{
	return mOwner->Scale().X() / mBaseScale;
}

float CBuffAura::CalcScaleY() const
{
	return mOwner->Scale().Y() / mBaseScale * 0.5f;
}

// �ҋ@��
void CBuffAura::UpdateIdle()
{
}

// �J�n
void CBuffAura::UpdateStart()
{
	float scaleXZ = CalcScaleXZ();
	float scaleY = CalcScaleY();

	// �o�ߎ��Ԃɍ��킹�đ傫������
	if (mElapsedTime < SCALE_TIME)
	{
		float percent = mElapsedTime / SCALE_TIME;
		scaleXZ *= percent;
		scaleY *= percent;
		Scale(scaleXZ, scaleY, scaleXZ);

		mElapsedTime += Time::DeltaTime();
	}
	else
	{
		Scale(scaleXZ, scaleY, scaleXZ);
		mElapsedTime -= SCALE_TIME;
		ChangeState(EState::Wait);
	}
}

// �I���҂�
void CBuffAura::UpdateWait()
{
	float scaleXZ = CalcScaleXZ();
	float scaleY = CalcScaleY();

	// �T�C���J�[�u�ŃX�P�[���l�̑����l���Z�o
	float percent = mElapsedTime / ANIM_TIME;
	float alpha = sinf(M_PI * 1.0f * percent);
	scaleXZ += ANIM_SCALE * alpha;
	scaleY += ANIM_SCALE * alpha;
	Scale(scaleXZ, scaleY, scaleXZ);

	mElapsedTime += Time::DeltaTime();
	if (mElapsedTime >= ANIM_TIME)
	{
		mElapsedTime -= ANIM_TIME;
	}
}

// �I��
void CBuffAura::UpdateEnd()
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
void CBuffAura::Update()
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

	// �o�t�I�[���̒��S���W
	float scaleXZ = CalcScaleXZ();
	float scaleY = CalcScaleY();
	CVector center = mOwner->Position();
	CVector pos = CVector::zero;
	pos.Y(11.0f * scaleY); 
	Position(center + pos);
}