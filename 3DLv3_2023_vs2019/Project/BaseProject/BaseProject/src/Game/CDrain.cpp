#include "CDrain.h"
#include "CCharaBase.h"
#include "Easing.h"
#include "Maths.h"
#include "CLich.h"
#include "CPlayer.h"

// �h���C���̃X�P�[���l�̍ő�l
#define SCALE 8.0f
// �h���C���̃X�P�[���l���ő�l�ɂȂ�܂ł̎���
#define SCALE_ANIM_TIME 0.001f
// �h���C���̔��˂̈ړ����x
#define MOVE_SPEED 0.8 *60.0f
// �v���C���[�܂ł̋���
#define DISTANCE 20.0f

// �R���X�g���N�^
CDrain::CDrain(ETag tag)
	: CBillBoardImage("Effect/heart.png", ETag::eDrain, ETaskPauseType::eGame)
	, mMoveDir(CVector::zero)
	, mIsDeath(false)
	, mMovedDist(0.0f)
	, mIsDrain(true)
	, mElapsedTime(0.0f)
	, mElapsedDrainTime(0.0f)
{
	mpAttackCollider = new CColliderSphere
	(
		this,
		ELayer::eAttackCol,
		1.0
	);
	mpAttackCollider->SetCollisionTags({ ETag::eField, ETag::eRideableObject,ETag::ePlayer });
	mpAttackCollider->SetCollisionLayers({ ELayer::eField ,ELayer::eDamageCol });
}

// �f�X�g���N�^
CDrain::~CDrain()
{
	SAFE_DELETE(mpAttackCollider);
}

// �e�p�����[�^��ݒ�
void CDrain::Setup(const CVector& pos, const CVector& dir, float dist)
{
	Position(pos);
	mKillMoveDist = dist;
}

// �폜�t���O�������Ă��邩�ǂ���
bool CDrain::IsDeath() const
{
	return mIsDeath;
}

// �J���[��ݒ�
void CDrain::SetColor(const CColor& color)
{
	mMaterial.Diffuse()[0] = color.R();
	mMaterial.Diffuse()[1] = color.G();
	mMaterial.Diffuse()[2] = color.B();
	mMaterial.Diffuse()[3] = color.A();
	CObjectBase::SetColor(color);
}

// �u�����h�^�C�v��ݒ�
void CDrain::SetBlendType(EBlend type)
{
	mMaterial.SetBlendType(type);
}

// �Փˏ���
void CDrain::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (other->Layer() == ELayer::eField)
	{
		Kill();
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
				int damage = CalcDamage(0.4f, mOwner, chara);

				// �_���[�W��^����
				chara->TakeDamage(damage, mOwner);

				// �U���ς݃��X�g�ɒǉ�
				AddAttackHitObj(chara);

				CLich* lich = CLich::Instance();
				lich->DrainHp();

				Kill();
			}
		}
	}
}

// �X�V
void CDrain::Update()
{
	// ���N���X�̍X�V����
	CBillBoardImage::Update();

	// �X�P�[���ύX���Ԃ��o�߂��Ă��Ȃ�
	if (mElapsedTime < SCALE_ANIM_TIME)
	{
		// �o�ߎ��Ԃɍ��킹�āA���X�Ƀh���C����傫������
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

	if (mIsDrain)
	{
		CPlayer* player = CPlayer::Instance();
		bool IsRolling = player->mRolling;

		// ���g����v���C���[�܂ł̃x�N�g��
		CVector targetPos = player->Position() + CVector(0.0f, 20.0f, 0.0f);
		CVector vec = targetPos - Position();

		float playerDist = vec.Length();
		if (IsRolling && playerDist <= DISTANCE)
		{
			// �h���C���I��
			mIsDrain = false;
		}
		else
		{
			mMoveDir = CVector::Slerp(mMoveDir, vec.Normalized(), 0.05f);
		}
	}

	// �ړ��x�N�g�������߂�
	CVector move = mMoveDir * MOVE_SPEED * Time::DeltaTime();

	float moveDist = move.Length();
	if (mMovedDist + moveDist >= mKillMoveDist)
	{
		moveDist = mKillMoveDist - mMovedDist;
		move = move.Normalized() * moveDist;
	}

	Position(Position() + move);

	if (mIsDrain == false)
	{
		mMovedDist += moveDist;
		mElapsedDrainTime += Time::DeltaTime();

		if (mMovedDist >= mKillMoveDist || mElapsedDrainTime >= 5.0f)
		{
			mElapsedDrainTime = 0.0f;
			Kill();
		}
	}
}