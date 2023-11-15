#include "CSword.h"
#include "CCollisionManager.h"
#include "CCharaBase.h"
#include "Maths.h"

CSword::CSword()
{
	mpSword = new CModel();
	mpSword->Load("Item\\Equipment\\Sword\\Sword.obj", "Item\\Equipment\\Sword\\Sword.mtl");

	//�U������p�̃R���C�_�[���쐬
	mpAttackCol = new CColliderLine
	(
		this, ELayer::eAttackCol,
		CVector(0.15f, 0.15f, 0.0f),
		CVector(0.8f,0.8f, 0.0f)
	);
	ChangeLevel(1);

	// �U������p�̃R���C�_�[�ƏՓ˔�����s��
	//���C���[�ƃ^�O��ݒ�
	mpAttackCol->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackCol->SetCollisionTags({ ETag::eEnemy });

	// �ŏ��͍U������p�̃R���C�_�[���I�t�ɂ��Ă���
	mpAttackCol->SetEnable(false);
}

CSword::~CSword()
{
	SAFE_DELETE(mpSword);
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
			//���ɍU���ς݂̃L�����łȂ����
			if (!IsAttackHitObj(chara))
			{
				// �_���[�W��^����
				chara->TakeDamage(mCharaMaxStatus.power,mOwner);

				//�U���ς݃��X�g�ɒǉ�
				AddAttackHitObj(chara);
			}
		}
	}
}

//����̍s��擾
CMatrix CSword::Matrix() const
{
	//��Ɏ����Ă��Ȃ��Ƃ��͎������g�̍s���Ԃ�
	if (mpAttachMtx == nullptr)
	{
		return CTransform::Matrix();
	}
	//��Ɏ����Ă���Ƃ��́A�A�^�b�`���Ă���s���Ԃ�
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
	//�U�����n�܂�����A�U������p�̃R���C�_�[���I���ɂ���
	mpAttackCol->SetEnable(true);
}

//�U���I��
void CSword:: AttackEnd()
{
	CWeapon::AttackEnd();
	//�U�����I���΁A�U������p�̃R���C�_�[���I�t�ɂ���
	mpAttackCol->SetEnable(false);
}

//1���x���A�b�v
void CSword::LevelUp()
{
	int level = mCharaStatus.level;
	ChangeLevel(level + 1);
}

//���x����ύX
void CSword::ChangeLevel(int level)
{
	//�X�e�[�^�X�̃e�[�u���̃C���f�b�N�X�l�ɕϊ�
	int index = Math::Clamp(level - 1, 0, PLAYER_LEVEL_MAX);
	//�ő�X�e�[�^�X�ɐݒ�
	mCharaMaxStatus = PLAYER_STATUS[index];
}