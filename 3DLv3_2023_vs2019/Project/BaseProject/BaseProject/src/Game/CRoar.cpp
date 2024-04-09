#include "CRoar.h"
#include "CCharaBase.h"
#include "Easing.h"

// �Y���т̃X�P�[���l�̍ő�l
#define FLAME_SCALE 60.0f
// �Y���т̃X�P�[���l���ő�l�ɂȂ�܂ł̎���
#define FLAME_SCALE_ANIM_TIME 3.0f

// �R���X�g���N�^
CRoar::CRoar(ETag tag)
	: CBillBoardImage("Effect/ring.png", ETag::eFlame, ETaskPauseType::eGame)
	, mMoveSpeed(CVector::zero)
	, mElapsedTime(0.0f)
	, mIsDeath(false)
{
}

// �f�X�g���N�^
CRoar::~CRoar()
{
}

// �e�p�����[�^��ݒ�
void CRoar::Setup(const CVector& pos, const CVector& dir, float speed)
{
	Position(pos);
	mMoveSpeed = dir.Normalized() * speed;
}

// �폜�t���O�������Ă��邩�ǂ���
bool CRoar::IsDeath() const
{
	return mIsDeath;
}

// �J���[��ݒ�
void CRoar::SetColor(const CColor& color)
{
	mMaterial.Diffuse()[0] = color.R();
	mMaterial.Diffuse()[1] = color.G();
	mMaterial.Diffuse()[2] = color.B();
	mMaterial.Diffuse()[3] = color.A();
	CObjectBase::SetColor(color);
}

// �u�����h�^�C�v��ݒ�
void CRoar::SetBlendType(EBlend type)
{
	mMaterial.SetBlendType(type);
}

// �Փˏ���
void CRoar::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (other->Layer() == ELayer::eField)
	{
		float length = mMoveSpeed.Length();
		CVector n = hit.adjust.Normalized();
		float d = CVector::Dot(n, mMoveSpeed);
		mMoveSpeed = (mMoveSpeed - n * d).Normalized() * length;
		Position(Position() + hit.adjust * hit.weight);
	}
}

// �X�V
void CRoar::Update()
{
	// ���N���X�̍X�V����
	CBillBoardImage::Update();

	// �Y���т̃G�t�F�N�g���ړ�
	CVector move = mMoveSpeed * Time::DeltaTime();
	Position(Position() + move);

	// �X�P�[���ύX���Ԃ��o�߂��Ă��Ȃ�
	if (mElapsedTime < FLAME_SCALE_ANIM_TIME)
	{
		// �o�ߎ��Ԃɍ��킹�āA���X�ɗY���т�傫������
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

	if (mElapsedTime >= 2)
	{
		Kill();
	}
}