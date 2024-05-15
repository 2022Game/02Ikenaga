#include "CSlashing.h"
#include "CCharaBase.h"
#include "Easing.h"

// �a���̃X�P�[���l�̍ő�l
#define FLAME_SCALE 15.0f
// �a���̃X�P�[���l���ő�l�ɂȂ�܂ł̎���
#define FLAME_SCALE_ANIM_TIME 3.0f

// �R���X�g���N�^
CSlashing::CSlashing(ETag tag)
	: CBillBoardImage("Effect/Slash.png", ETag::eFlame, ETaskPauseType::eGame)
	,LifeTime(0)
	, mMoveSpeed(CVector::zero)
	, mElapsedTime(0.0f)
	, mIsDeath(false)
{

	mpColliderLine = new CColliderLine
	(
		this,ELayer::eAttackCol,
		CVector(1.0f,0.0f,0.0f),
		CVector(0.0f, -1.0f, 0.0f),
		false,1.0f
	);
	mpColliderLine->SetCollisionTags({ ETag::eField, ETag::eRideableObject,ETag::eEnemy });
	mpColliderLine->SetCollisionLayers({ ELayer::eField ,ELayer::eDamageCol });
	//mpCollider->Position(-10.0f, -15.0f, 0.0f);
	//mpCollider->Rotation(-20.0f, 0.0f, 0.0f);
}

// �f�X�g���N�^
CSlashing::~CSlashing()
{
	SAFE_DELETE(mpColliderLine);
}

// �e�p�����[�^��ݒ�
void CSlashing::Setup(const CVector& pos, const CVector& dir, float speed)
{
	Position(pos);
	mMoveSpeed = dir.Normalized() * speed;
}

// �폜�t���O�������Ă��邩�ǂ���
bool CSlashing::IsDeath() const
{
	return mIsDeath;
}

// �J���[��ݒ�
void CSlashing::SetColor(const CColor& color)
{
	mMaterial.Diffuse()[0] = color.R();
	mMaterial.Diffuse()[1] = color.G();
	mMaterial.Diffuse()[2] = color.B();
	mMaterial.Diffuse()[3] = color.A();
	CObjectBase::SetColor(color);
}

// �u�����h�^�C�v��ݒ�
void CSlashing::SetBlendType(EBlend type)
{
	mMaterial.SetBlendType(type);
}

// �Փˏ���
void CSlashing::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
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
			// �^����_���[�W���v�Z
			int damage = CalcDamage(1.0f,0, chara);

			// �_���[�W��^����
			chara->TakeDamage(damage, 0);
		}
	}
}

// �X�V
void CSlashing::Update()
{
	// ���N���X�̍X�V����
	CBillBoardImage::Update();

	// �a���̃G�t�F�N�g���ړ�
	CVector move = mMoveSpeed * Time::DeltaTime();
	Position(Position() + move);

	// �X�P�[���ύX���Ԃ��o�߂��Ă��Ȃ�
	if (mElapsedTime < FLAME_SCALE_ANIM_TIME)
	{
		// �o�ߎ��Ԃɍ��킹�āA���X�ɃX���b�V����傫������
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

	if (mElapsedTime >= 0.7)
	{
		Kill();
	}

	CDebugPrint::Print(" �o�ߎ��� %f", mElapsedTime);
}