#include "CImpact.h"
#include "CCharaBase.h"
#include "Easing.h"

// �Ռ��̃X�P�[���l�̍ő�l
#define FLAME_SCALE 8.0f
// �Ռ��̃X�P�[���l���ő�l�ɂȂ�܂ł̎���
#define FLAME_SCALE_ANIM_TIME 3.0f

// �A�j���[�V������1�R�}�\������
#define ANIM_TIME 0.0625f
// �Ռ��̃G�t�F�N�g�̃A�j���[�V�����f�[�^
TexAnimData CImpact::msAnimData = TexAnimData(3, 5, false, 64, ANIM_TIME);

// �R���X�g���N�^
CImpact::CImpact(ETag tag)
	: CBillBoardImage("Effect/Impact.png", ETag::eImpact, ETaskPauseType::eGame)
	, mMoveSpeed(CVector::zero)
	, mElapsedTime(0.0f)
	, mIsDeath(false)
{
	SetAnimData(&msAnimData);

	mpCollider = new CColliderSphere
	(
		this,
		ELayer::eAttackCol,
		2.0
	);
	mpCollider->SetCollisionTags({ ETag::eField, ETag::eRideableObject,ETag::ePlayer });
	mpCollider->SetCollisionLayers({ ELayer::eField ,ELayer::eDamageCol });
}

// �f�X�g���N�^
CImpact::~CImpact()
{
	SAFE_DELETE(mpCollider);
}

// �e�p�����[�^��ݒ�
void CImpact::Setup(const CVector& pos, const CVector& dir, float speed)
{
	Position(pos);
	mMoveSpeed = dir.Normalized() * speed;
}

// �폜�t���O�������Ă��邩�ǂ���
bool CImpact::IsDeath() const
{
	return mIsDeath;
}

// �J���[��ݒ�
void CImpact::SetColor(const CColor& color)
{
	mMaterial.Diffuse()[0] = color.R();
	mMaterial.Diffuse()[1] = color.G();
	mMaterial.Diffuse()[2] = color.B();
	mMaterial.Diffuse()[3] = color.A();
	CObjectBase::SetColor(color);
}

// �u�����h�^�C�v��ݒ�
void CImpact::SetBlendType(EBlend type)
{
	mMaterial.SetBlendType(type);
}

// �Փˏ���
void CImpact::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
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
			//if (!IsAttackHitObj(chara))
			//{
				// �^����_���[�W���v�Z
			int damage = CalcDamage(1.0f,0, chara);

			// �_���[�W��^����
			chara->TakeDamage(damage, 0);

			// �U���ς݃��X�g�ɒǉ�
		   // AddAttackHitObj(chara);
		//}
		}
	}
}

// �X�V
void CImpact::Update()
{
	// ���N���X�̍X�V����
	CBillBoardImage::Update();

	// �Ռ��̃G�t�F�N�g���ړ�
	CVector move = mMoveSpeed * Time::DeltaTime();
	Position(Position() + move);

	// �X�P�[���ύX���Ԃ��o�߂��Ă��Ȃ�
	if (mElapsedTime < FLAME_SCALE_ANIM_TIME)
	{
		// �o�ߎ��Ԃɍ��킹�āA���X�ɏՌ���傫������
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
	// �A�j���[�V�������I�������A�폜�t���O�𗧂Ă�
	if (IsEndAnim())
	{
		mIsDeath = true;
	}
	if (mElapsedTime >= 0.5)
	{
		Kill();
	}
}