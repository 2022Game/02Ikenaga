#include "CLightningBall.h"
#include "CCharaBase.h"
#include "Easing.h"
#include "Maths.h"

// �����̃X�P�[���l�̍ő�l
#define FLAME_SCALE 80.0f
// �����̃X�P�[���l���ő�l�ɂȂ�܂ł̎���
#define FLAME_SCALE_ANIM_TIME 3.0f

// �A�j���[�V������1�R�}�\������
//#define ANIM_TIME 0.0625f
// ���̃G�t�F�N�g�̃A�j���[�V�����f�[�^
//TexAnimData CLightningBall::msAnimData = TexAnimData(7, 2, false, 64, ANIM_TIME);

CLightningBall* CLightningBall::spInstance = nullptr;

// �C���X�^���X
CLightningBall* CLightningBall::Instance()
{
	return spInstance;
}

// �R���X�g���N�^
CLightningBall::CLightningBall(ETag tag)
	: CBillBoardImage("Effect/Ball.png", ETag::eLightningBall, ETaskPauseType::eGame)
	, mMoveSpeed(CVector::zero)
	, mElapsedTime(0.0f)
	, mIsDeath(false)
{
	//�C���X�^���X�̐ݒ�
	spInstance = this;

	mpCollider = new CColliderSphere
	(
		this,
		ELayer::eAttackCol,
		0.14
	);
	mpCollider->SetCollisionTags({ ETag::eField, ETag::eRideableObject,ETag::ePlayer });
	mpCollider->SetCollisionLayers({ ELayer::eField ,ELayer::eDamageCol });
}

// �f�X�g���N�^
CLightningBall::~CLightningBall()
{
	SAFE_DELETE(mpCollider);
}

// �e�p�����[�^��ݒ�
void CLightningBall::Setup(const CVector& pos, const CVector& dir, float speed)
{
	Position(pos);
	mMoveSpeed = dir.Normalized() * speed;
}

// �폜�t���O�������Ă��邩�ǂ���
bool CLightningBall::IsDeath() const
{
	return mIsDeath;
}

// �J���[��ݒ�
void CLightningBall::SetColor(const CColor& color)
{
	mMaterial.Diffuse()[0] = color.R();
	mMaterial.Diffuse()[1] = color.G();
	mMaterial.Diffuse()[2] = color.B();
	mMaterial.Diffuse()[3] = color.A();
	CObjectBase::SetColor(color);
}

// �u�����h�^�C�v��ݒ�
void CLightningBall::SetBlendType(EBlend type)
{
	mMaterial.SetBlendType(type);
}

// �Փˏ���
void CLightningBall::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (other->Layer() == ELayer::eField)
	{
		float length = mMoveSpeed.Length();
		CVector n = hit.adjust.Normalized();
		float d = CVector::Dot(n, mMoveSpeed);
		mMoveSpeed = (mMoveSpeed - n * d).Normalized() * length;
		Position(Position() + hit.adjust * hit.weight);
	}

	if (other->Layer() == ELayer::eDamageCol)
	{
		// �L�����̃|�C���^�ɕϊ�
		CCharaBase* chara = dynamic_cast<CCharaBase*> (other->Owner());
		// ����̃R���C�_�[�̎����傪�L�����ł���΁A
		if (chara != nullptr)
		{
			// ���ɍU���ς݂̃L�����łȂ����
			if (!IsAttackHitObj(chara))
			{
				// �^����_���[�W���v�Z
				int damage = CalcDamage(mOwner, chara);

				// �_���[�W��^����
				chara->TakeDamage(damage, mOwner);

				// �U���ς݃��X�g�ɒǉ�
				AddAttackHitObj(chara);
			}
		}
	}
}

// �X�V
void CLightningBall::Update()
{
	// ���N���X�̍X�V����
	CBillBoardImage::Update();

	// �����̃G�t�F�N�g���ړ�
	CVector move = mMoveSpeed * Time::DeltaTime();
	Position(Position() + move);

	// �X�P�[���ύX���Ԃ��o�߂��Ă��Ȃ�
	if (mElapsedTime < FLAME_SCALE_ANIM_TIME)
	{
		// �o�ߎ��Ԃɍ��킹�āA���X�ɗ�����傫������
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