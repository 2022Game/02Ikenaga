#include "CWave.h"
#include "CCharaBase.h"

// �R���X�g���N�^
CWave::CWave(CObjectBase* owner, const CVector& pos, const CVector& dir,
	float speed, float dist)
	: mpOwner(owner)
	, mKillMoveDist(dist)
	, mMovedDist(0.0f)

{
	Position(pos);
	mMoveSpeed = dir.Normalized() * speed;
	Rotation(CQuaternion::LookRotation(mMoveSpeed, CVector::up));

	mpModel = CResourceManager::Get<CModel>("Wave");
	mpModel->SetupEffectSettings();

	mpAttackCol = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		9.0f,false
	);
	mpAttackCol->SetCollisionTags({ ETag::eField, ETag::eRideableObject,ETag::ePlayer });
	mpAttackCol->SetCollisionLayers({ ELayer::eField ,ELayer::eDamageCol });
}

// �f�X�g���N�^
CWave::~CWave()
{
	SAFE_DELETE(mpAttackCol);
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
				int damage = CalcDamage(0.7f, mOwner, chara);

				// �_���[�W��^����
				chara->TakeDamage(damage, mOwner);

				// �U���ς݃��X�g�ɒǉ�
				AddAttackHitObj(chara);
			}
		}
	}
}

// �U���J�n
void CWave::AttackStart()
{
	CWeapon::AttackStart();
	// �U�����n�܂�����A�U������p�̃R���C�_�[���I���ɂ���
	mpAttackCol->SetEnable(true);
}

// �U���I��
void CWave::AttackEnd()
{
	CWeapon::AttackEnd();
	// �U�����I���΁A�U������p�̃R���C�_�[���I�t�ɂ���
	mpAttackCol->SetEnable(false);
}

// �X�V
void CWave::Update()
{
	CVector move = mMoveSpeed * Time::DeltaTime();
	float dist = move.Length();
	if (mMovedDist + dist >= mKillMoveDist)
	{
		dist = mKillMoveDist - mMovedDist;
		move = move.Normalized() * dist;
	}
	Position(Position() + move);

	mMovedDist += dist;
	if (mMovedDist >= mKillMoveDist)
	{
		Kill();
	}
}

// �`��
void CWave::Render()
{
	mpModel->SetColor(mColor);
	mpModel->Render(Matrix());
}