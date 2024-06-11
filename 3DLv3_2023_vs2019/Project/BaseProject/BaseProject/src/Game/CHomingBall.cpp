#include "CHomingBall.h"
#include "CCharaBase.h"
#include "Easing.h"
#include "Maths.h"
#include "CPlayer.h"

// �z�[�~���O�{�[���̃X�P�[���l�̍ő�l
#define SCALE 30.0f
// �z�[�~���O�{�[���̃X�P�[���l���ő�l�ɂȂ�܂ł̎���
#define SCALE_ANIM_TIME 3.0f
// �z�[�~���O�{�[���̔��˂̈ړ����x
#define MOVE_SPEED 0.75f

CHomingBall* CHomingBall::spInstance = nullptr;

// �C���X�^���X
CHomingBall* CHomingBall::Instance()
{
	return spInstance;
}

// �R���X�g���N�^
CHomingBall::CHomingBall(ETag tag)
	: CBillBoardImage("Effect/Ball.png", ETag::eHomingBall, ETaskPauseType::eGame)
	, mMoveSpeed(CVector::zero)
	, mElapsedTime(0.0f)
	, mIsDeath(false)
	, mMovedDist(0.0f)
	, mRolling(false)
{
	//�C���X�^���X�̐ݒ�
	spInstance = this;

	mpAttackCollider = new CColliderSphere
	(
		this,
		ELayer::eAttackCol,
		0.14
	);
	mpAttackCollider->SetCollisionTags({ ETag::eField, ETag::eRideableObject,ETag::ePlayer });
	mpAttackCollider->SetCollisionLayers({ ELayer::eField ,ELayer::eDamageCol });
}

// �f�X�g���N�^
CHomingBall::~CHomingBall()
{
	SAFE_DELETE(mpAttackCollider);
}

// �e�p�����[�^��ݒ�
void CHomingBall::Setup(const CVector& pos, const CVector& dir, float dist)
{
	Position(pos);
	mMoveSpeed = dir.Normalized() * MOVE_SPEED;
	mKillMoveDist = dist;
}

// �폜�t���O�������Ă��邩�ǂ���
bool CHomingBall::IsDeath() const
{
	return mIsDeath;
}

// �J���[��ݒ�
void CHomingBall::SetColor(const CColor& color)
{
	mMaterial.Diffuse()[0] = color.R();
	mMaterial.Diffuse()[1] = color.G();
	mMaterial.Diffuse()[2] = color.B();
	mMaterial.Diffuse()[3] = color.A();
	CObjectBase::SetColor(color);
}

// �u�����h�^�C�v��ݒ�
void CHomingBall::SetBlendType(EBlend type)
{
	mMaterial.SetBlendType(type);
}

// �Փˏ���
void CHomingBall::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
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
				int damage = CalcDamage(1.0f, mOwner, chara);

				// �_���[�W��^����
				chara->TakeDamage(damage, mOwner);

				// �U���ς݃��X�g�ɒǉ�
				AddAttackHitObj(chara);
				Kill();
			}
		}
	}
}

// �X�V
void CHomingBall::Update()
{
	// ���N���X�̍X�V����
	CBillBoardImage::Update();

	// �z�[�~���O�{�[���̃G�t�F�N�g���ړ�
	CVector move = mMoveSpeed * Time::DeltaTime();
	Position(Position() + move);

	// �X�P�[���ύX���Ԃ��o�߂��Ă��Ȃ�
	if (mElapsedTime < SCALE_ANIM_TIME)
	{
		// �o�ߎ��Ԃɍ��킹�āA���X�Ƀz�[�~���O�{�[����傫������
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

	float dist = move.Length();
	if (mMovedDist + dist >= mKillMoveDist)
	{
		dist = mKillMoveDist - mMovedDist;
		move = move.Normalized() * dist;
	}
	CPlayer* player = CPlayer::Instance();
	mRolling = player->mRolling;

	if (mRolling == false)
	{
		CVector newPos = (player->Position() - Position()).Normalized();
		float vectorPos = (player->Position() - Position()).Length();

		move.Y(0.3f);
		if (vectorPos >= 0.0f)
		{
			move += newPos * MOVE_SPEED;
		}
		Position(Position() + move);
	}
	else
	{
		Position(Position() + move);
	}

	mMovedDist += dist;
	if (mMovedDist >= mKillMoveDist)
	{
		Kill();
	}
}