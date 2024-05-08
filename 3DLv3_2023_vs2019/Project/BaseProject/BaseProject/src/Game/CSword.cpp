#include "CSword.h"
#include "CCollisionManager.h"
#include "CCharaBase.h"
#include "CPlayer.h"

CSword::CSword()
{
	mpSword = CResourceManager::Get<CModel>("Sword");

	// �U������p�̃R���C�_�[���쐬
	mpAttackCol = new CColliderLine
	(
		this, ELayer::eAttackCol,
		CVector(0.15f, 0.15f, 0.0f),
		CVector(0.8f,0.8f, 0.0f)
	);

	// �U������p�̃R���C�_�[�ƏՓ˔�����s��
	// ���C���[�ƃ^�O��ݒ�
	mpAttackCol->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackCol->SetCollisionTags({ ETag::eEnemy });

	// �ŏ��͍U������p�̃R���C�_�[���I�t�ɂ��Ă���
	mpAttackCol->SetEnable(false);
}

CSword::~CSword()
{
	SAFE_DELETE(mpAttackCol);
}

void CSword::Update()
{
}

void CSword::Render()
{
	mpSword->Render(Matrix());
}

// �Փˏ���
void CSword::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// �Փ˂��������̃R���C�_�[���U������p�̃R���C�_�[�ł���΁A
	if (self == mpAttackCol)
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
				chara->TakeDamage(damage,mOwner);

				// �U���ς݃��X�g�ɒǉ�
				AddAttackHitObj(chara);
				CPlayer* player = player->Instance();
				player->AttackRecovery();

			}
		}
	}
}

// ����̍s��擾
CMatrix CSword::Matrix() const
{
	// ��Ɏ����Ă��Ȃ��Ƃ��͎������g�̍s���Ԃ�
	if (mpAttachMtx == nullptr)
	{
		return CTransform::Matrix();
	}
	// ��Ɏ����Ă���Ƃ��́A�A�^�b�`���Ă���s���Ԃ�
	else
	{
		CMatrix sm;
		sm.Scale(100.0f, 100.0f, 100.0f);
		return sm * (*mpAttachMtx);
	}
}

// �U���J�n
void CSword::AttackStart()
{
	CWeapon::AttackStart();
	// �U�����n�܂�����A�U������p�̃R���C�_�[���I���ɂ���
	mpAttackCol->SetEnable(true);
}

// �U���I��
void CSword:: AttackEnd()
{
	CWeapon::AttackEnd();
	// �U�����I���΁A�U������p�̃R���C�_�[���I�t�ɂ���
	mpAttackCol->SetEnable(false);
}