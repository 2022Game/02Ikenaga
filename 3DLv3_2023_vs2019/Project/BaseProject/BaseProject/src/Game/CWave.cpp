#include "CWave.h"
#include "CCharaBase.h"
#include "Easing.h"
#include "CRay.h"
#include "Maths.h"

// �g���̃X�P�[���l�̍ő�l
#define FLAME_SCALE 40.0f
// �g���̃X�P�[���l���ő�l�ɂȂ�܂ł̎���
#define FLAME_SCALE_ANIM_TIME 3.0f

// �g���̊Ԋu����
#define THROW_INTERVAL 0.2f
// �g���̕����̃u����
#define FLAME_DIR_RAND 0.02f
// �g���̈ړ����x
#define FLAME_MOVE_SPEED 25.0f
// �g���̐F
#define FLAME_COLOR CColor(0.0f, 0.5f, 1.0f)


// �R���X�g���N�^
CWave::CWave(ETag tag)
	: CBillBoardImage("Effect/ring2.png", ETag::eWave, ETaskPauseType::eGame)
	, mMoveSpeed(CVector::zero)
	, mElapsedTime(0.0f)
	, mIsDeath(false)
{
	mpCollider = new CColliderSphere
	(
		this,
		ELayer::eAttackCol,
		0.2
	);
	mpCollider->SetCollisionTags({ ETag::eField, ETag::eRideableObject,ETag::ePlayer });
	mpCollider->SetCollisionLayers({ ELayer::eField ,ELayer::eDamageCol });
}

// �f�X�g���N�^
CWave::~CWave()
{
	SAFE_DELETE(mpCollider);
}

// �e�p�����[�^��ݒ�
void CWave::Setup(const CVector& pos, const CVector& dir, float speed)
{
	Position(pos);
	mMoveSpeed = dir.Normalized() * speed;
}

// �폜�t���O�������Ă��邩�ǂ���
bool CWave::IsDeath() const
{
	return mIsDeath;
}

// �J���[��ݒ�
void CWave::SetColor(const CColor& color)
{
	mMaterial.Diffuse()[0] = color.R();
	mMaterial.Diffuse()[1] = color.G();
	mMaterial.Diffuse()[2] = color.B();
	mMaterial.Diffuse()[3] = color.A();
	CObjectBase::SetColor(color);
}

// �u�����h�^�C�v��ݒ�
void CWave::SetBlendType(EBlend type)
{
	mMaterial.SetBlendType(type);
}

// �Փˏ���
void CWave::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
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
void CWave::Update()
{
	// ���N���X�̍X�V����
	CBillBoardImage::Update();

	// �g���̃G�t�F�N�g���ړ�
	CVector move = mMoveSpeed * Time::DeltaTime();
	Position(Position() + move);

	// �X�P�[���ύX���Ԃ��o�߂��Ă��Ȃ�
	if (mElapsedTime < FLAME_SCALE_ANIM_TIME)
	{
		// �o�ߎ��Ԃɍ��킹�āA���X�ɔg����傫������
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