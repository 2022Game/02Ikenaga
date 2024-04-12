#include "CCrack.h"
#include "CCharaBase.h"
#include "Easing.h"

// �Ђъ���̃X�P�[���l�̍ő�l
#define FLAME_SCALE 10.0f
// �Ђъ���̃X�P�[���l���ő�l�ɂȂ�܂ł̎���
#define FLAME_SCALE_ANIM_TIME 3.0f

// �R���X�g���N�^
CCrack::CCrack(ETag tag)
	: CBillBoardImage("Effect/Crack.png", ETag::eNone, ETaskPauseType::eGame)
	, mMoveSpeed(CVector::zero)
	, mElapsedTime(0.0f)
	, mIsDeath(false)
{
}

// �f�X�g���N�^
CCrack::~CCrack()
{
}

// �e�p�����[�^��ݒ�
void CCrack::Setup(const CVector& pos, const CVector& dir, float speed)
{
	Position(pos);
	mMoveSpeed = dir.Normalized() * speed;
}

// �폜�t���O�������Ă��邩�ǂ���
bool CCrack::IsDeath() const
{
	return mIsDeath;
}

// �J���[��ݒ�
void CCrack::SetColor(const CColor& color)
{
	mMaterial.Diffuse()[0] = color.R();
	mMaterial.Diffuse()[1] = color.G();
	mMaterial.Diffuse()[2] = color.B();
	mMaterial.Diffuse()[3] = color.A();
	CObjectBase::SetColor(color);
}

// �u�����h�^�C�v��ݒ�
void CCrack::SetBlendType(EBlend type)
{
	mMaterial.SetBlendType(type);
}

// �X�V
void CCrack::Update()
{
	// ���N���X�̍X�V����
	CBillBoardImage::Update();

	// �X�P�[���ύX���Ԃ��o�߂��Ă��Ȃ�
	if (mElapsedTime < FLAME_SCALE_ANIM_TIME)
	{
		// �o�ߎ��Ԃɍ��킹�āA���X�ɂЂъ����傫������
		float per = mElapsedTime / FLAME_SCALE_ANIM_TIME;
		if (per < 1.0f)
		{
			float scale = Easing::QuadOut(per, 1.0f, 1.0f, 1.0f);
			Scale(CVector::one * scale * FLAME_SCALE);
		}
		else
		{
			Scale(CVector::one * FLAME_SCALE);
		}

		mElapsedTime += Time::DeltaTime();
	}
	// �ړ����Ԃ��o�߂�����A�폜����
	else
	{
		Scale(CVector::one * FLAME_SCALE);
	}

	if (mElapsedTime >= 1)
	{
		Kill();
	}
}