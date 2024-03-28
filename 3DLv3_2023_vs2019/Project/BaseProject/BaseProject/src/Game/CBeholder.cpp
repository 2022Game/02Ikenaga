#include "CBeholder.h"
#include "CPlayer.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "CInput.h"
#include "Maths.h"

// ���̂̃����X�^�[�̃C���X�^���X
CBeholder* CBeholder::spInstance = nullptr;

#define ENEMY_HEIGHT 1.0f
#define WITHIN_RANGE 40.0f    // �͈͓�

// ���̂̃����X�^�[�̃A�j���[�V�����f�[�^�̃e�[�u��
const CBeholder::AnimData CBeholder::ANIM_DATA[] =
{
	{ "",										true,	0.0f	},// T�|�[�Y
	{ "Character\\Enemy\\Beholder\\animation\\BeholderIdle2.x",	true,	46.0f	},	    // �ҋ@ 23.0f
	{ "Character\\Enemy\\Beholder\\animation\\BeholderIdle.x",	true,	82.0f	},	    // �ҋ@2 41.0f
	{ "Character\\Enemy\\Beholder\\animation\\BeholderAttack.x",	true,	46.0f	},	    // �U�� 23.0f
	//{ "Character\\Enemy\\Beholder\\animation\\BeholderAttack2.x",	true,	42.0f	},	    // �U��2 21.0f
	//{ "Character\\Enemy\\Beholder\\animation\\BeholderAttack3.x",	true,	42.0f	},	    // �U��3 17.0f
	//{ "Character\\Enemy\\Beholder\\animation\\BeholderAttack4.x",	true,	46.0f	},	    // �U��4 23.0f
	//{ "Character\\Enemy\\Beholder\\animation\\BeholderDie.x",	true,	76.0f	},	        // ���� 23.0f
	//{ "Character\\Enemy\\Beholder\\animation\\BeholderDizzy.x",	true,	82.0f	},	    // �߂܂� 41.0f
	//{ "Character\\Enemy\\Beholder\\animation\\BeholderGetHit.x",	true,	46.0f	},	    // �q�b�g 23.0f
	//{ "Character\\Enemy\\Beholder\\animation\\BeholderRun.x",	true,	34.0f	},	    // ���� 17.0f
};

// �R���X�g���N�^
CBeholder::CBeholder()
	: mpRideObject(nullptr)
	, mAttackTime(0)
{
	//�C���X�^���X�̐ݒ�
	spInstance = this;

	// ���f���f�[�^�ǂݍ���
	CModelX* model = CResourceManager::Get<CModelX>("Beholder");

	//�ŏ���1���x���ɐݒ�
	ChangeLevel(1);

	// �e�[�u�����̃A�j���[�V�����f�[�^��ǂݍ���
	int size = ARRAY_SIZE(ANIM_DATA);
	for (int i = 0; i < size; i++)
	{
		const AnimData& data = ANIM_DATA[i];
		if (data.path.empty()) continue;
		model->AddAnimationSet(data.path.c_str());
	}
	// CXCharacter�̏�����
	Init(model);

	// �ŏ��͑ҋ@�A�j���[�V�������Đ�
	ChangeAnimation(EAnimType::eIdle);

	//mpColliderLine = new CColliderLine
	//(
	//	this, ELayer::eField,
	//	CVector(0.0f, 0.0f, 0.0f),
	//	CVector(0.0f, ENEMY_HEIGHT, 0.0f)
	//);
	//mpColliderLine->SetCollisionLayers({ ELayer::eField });

	//// �L�����N�^�[�����߂�����
	//mpColliderSphere = new CColliderSphere
	//(
	//	this, ELayer::eEnemy,
	//	0.3f, false, 5.0f
	//);
	//mpColliderSphere->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy2 });
	//mpColliderSphere->Position(0.0f, 0.2f, 0.0f);

	//// �_���[�W���󂯂�R���C�_�[���쐬
	//mpDamageCol = new CColliderSphere
	//(
	//	this, ELayer::eDamageCol,
	//	0.4f, false
	//);
	////�@�_���[�W���󂯂�R���C�_�[��
	////�@�Փ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	//mpDamageCol->SetCollisionLayers({ ELayer::eAttackCol });
	//mpDamageCol->SetCollisionTags({ ETag::eWeapon });
	////�_���[�W���󂯂�R���C�_�[��������ւ��炷
	//mpDamageCol->Position(0.0f, 0.3f, 0.0f);

	//// �_���[�W��^����R���C�_�[
	//mpAttackCol = new CColliderSphere
	//(
	//	this, ELayer::eAttackCol,
	//	0.55f, false
	//);
	//mpAttackCol->SetCollisionLayers({ ELayer::eDamageCol });
	//mpAttackCol->SetCollisionTags({ ETag::ePlayer });
	//mpAttackCol->Position(0.0f, -0.1f, 0.0f);

	//// �U���R���C�_�[���}�b�V�����[���̓��̍s��ɃA�^�b�`
	//const CMatrix* headMty = GetFrameMtx("Armature_mushroom_spine03");
	//mpAttackCol->SetAttachMtx(headMty);

	//// �ŏ��̍U���R���C�_�[�𖳌��ɂ��Ă���
	//mpAttackCol->SetEnable(false);
}

CBeholder::~CBeholder()
{
	/*SAFE_DELETE(mpColliderLine);
	SAFE_DELETE(mpColliderSphere);
	SAFE_DELETE(mpDamageCol);
	SAFE_DELETE(mpAttackCol);*/
}

CBeholder* CBeholder::Instance()
{
	return spInstance;
}

// �A�j���[�V�����؂�ւ�
void CBeholder::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// �ҋ@���
void CBeholder::UpdateIdle()
{
	ChangeAnimation(EAnimType::eIdle);
	if (IsAnimationFinished())
	{
		mState = EState::eIdle;
	}
}

// �ҋ@���2
void CBeholder::UpdateIdle2()
{
	ChangeAnimation(EAnimType::eIdle2);
	if (IsAnimationFinished())
	{
		mState = EState::eIdle2;
	}
}

// �U��
void CBeholder::UpdateAttack()
{
	ChangeAnimation(EAnimType::eAttack);
	AttackStart();
	// �U��2�I���҂���Ԃֈڍs
	mState = EState::eAttackWait;
}

// �U��2
void CBeholder::UpdateAttack2()
{
	ChangeAnimation(EAnimType::eAttack2);
	AttackStart();
	// �U��2�I���҂���Ԃֈڍs
	mState = EState::eAttackWait;
}

// �U���I���҂�
void CBeholder::UpdateAttackWait()
{
	if (IsAnimationFinished())
	{
		AttackEnd();
		mState = EState::eIdle2;
	}
}

// �q�b�g
void CBeholder::UpdateHit()
{
	// �q�b�g�A�j���[�V�������J�n
	ChangeAnimation(EAnimType::eHit);
	if (IsAnimationFinished())
	{
		// �߂܂���false�ɂ���
		bool stan = false;
		// �m�����ŏ���0�ő�40
		int probability = Math::Rand(0, 20);
		if (probability == 1)stan = true;
		if (stan)
		{
			//mState = EState::eDizzy;
		}
		else
		{
			// �v���C���[�̍U�����q�b�g�������̑ҋ@��Ԃֈڍs
			mState = EState::eIdle2;
			ChangeAnimation(EAnimType::eIdle2);
		}
	}
}

// ����
void CBeholder::UpdateDie()
{
	//ChangeAnimation(EAnimType::eDie);
}

// �߂܂�(����)
void CBeholder::UpdateDizzy()
{
	//ChangeAnimation(EAnimType::eDizzy);
}

// �X�V����
void CBeholder::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;

	// ��Ԃɍ��킹�āA�X�V������؂�ւ���
	switch (mState)
	{
		// �ҋ@���
	case EState::eIdle:
		UpdateIdle();
		break;
		// �ҋ@���2
	case EState::eIdle2:
		UpdateIdle2();
		break;
		// �U��
	case EState::eAttack:
		UpdateAttack();
		break;
		// �U��2
	case EState::eAttack2:
		UpdateAttack2();
		break;
		// �U���I���҂�
	case EState::eAttackWait:
		UpdateAttackWait();
		break;
		// �q�b�g
	case EState::eHit:
		UpdateHit();
		break;
		// ����
	case EState::eDie:
		UpdateDie();
		break;
		// �߂܂�(����)
	case EState::eDizzy:
		UpdateDizzy();
		break;
	}

	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();
	if (vectorp <= WITHIN_RANGE && mState != EState::eIdle && mState != EState::eIdle2
		&& mState != EState::eAttack && mState != EState::eAttackWait)
	{
		mState = EState::eIdle2;
	}
	if (mState == EState::eIdle2)
	{
		mAttackTime++;

		if (mAttackTime > 200)
		{
			// �U��2
			//bool Attack2 = false;
			//// �U��3
			//bool Attack3 = false;
			//// �m�����ŏ���3�ő�6
			//int probability2 = Math::Rand(2, 5);
			//int probability3 = Math::Rand(6, 10);
			//if (probability2 == 2)Attack2 = true;
			//if (probability3 == 6)Attack3 = true;
			//if (Attack2)
			//{
			//	mState = EState::eAttack2;
			//}
			//else if (Attack3)
			//{
			//	mState = EState::eAttack3;
			//}
			//else
			//{
				mState = EState::eAttack;
			//}
		}
		if (mState == EState::eAttack || mState == EState::eAttack2)
		{
			mAttackTime = 0;
		}
	}

	//CDebugPrint::Print(" �U������: %d\n", mAttackTime);
	//CDebugPrint::Print(" HP: %d\n", mCharaStatus.hp);

	// �L�����N�^�[�̍X�V
	CXCharacter::Update();

	//mpAttackCol->Update();

	mIsGrounded = false;
}

// �Փˏ���
void CBeholder::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// �Փ˂��������̃R���C�_�[���U������p�̃R���C�_�[�ł���΁A
	//if (self == mpAttackCol && mState != EState::eIdle && mState != EState::eIdle2 &&
	//	mState != EState::eIdle3)
	//{
	//	// �L�����̃|�C���^�ɕϊ�
	//	CCharaBase* chara = dynamic_cast<CCharaBase*> (other->Owner());
	//	// ����̃R���C�_�[�̎����傪�L�����ł���΁A
	//	if (chara != nullptr)
	//	{
	//		// ���ɍU���ς݂̃L�����łȂ����
	//		if (!IsAttackHitObj(chara))
	//		{
	//			int damage = CalcDamage(this, chara);

	//			// �_���[�W��^����
	//			chara->TakeDamage(damage, this);

	//			// �U���ς݃��X�g�ɒǉ�
	//			AddAttackHitObj(chara);
	//		}
	//	}
	//}
	//else if (self == mpColliderLine)
	//{
	//	if (other->Layer() == ELayer::eField)
	//	{
	//		Position(Position() + hit.adjust * hit.weight);
	//		mIsGrounded = true;
	//		//mMoveSpeed.Y(0.0f);

	//		if (other->Tag() == ETag::eRideableObject)
	//		{
	//			mpRideObject = other->Owner();
	//		}
	//	}
	//}
	// �L�����N�^�[���m�̏Փˏ���
	/*else if (self == mpColliderSphere)
	{
		CVector pushBack = hit.adjust * hit.weight;
		pushBack.Y(0.0f);
		Position(Position() + pushBack);
	}*/
}

// �U���J�n
void CBeholder::AttackStart()
{
	//CXCharacter::AttackStart();
	// �U�����n�܂�����A�U������p�̃R���C�_�[���I���ɂ���
	//mpAttackCol->SetEnable(true);
}

// �U���I��
void CBeholder::AttackEnd()
{
	//CXCharacter::AttackEnd();
	// �U�����I���΁A�U������p�̃R���C�_�[���I�t�ɂ���
	//mpAttackCol->SetEnable(false);
}

// �`��
void CBeholder::Render()
{
	CXCharacter::Render();
}

// 1���x���A�b�v
void CBeholder::LevelUp()
{
	int level = mCharaStatus.level;
	ChangeLevel(level + 1);
}

// ���x����ύX
void CBeholder::ChangeLevel(int level)
{
	// �X�e�[�^�X�̃e�[�u���̃C���f�b�N�X�l�ɕϊ�
	int index = Math::Clamp(level - 1, 0, ENEMY__LEVEL_MAX);
	// �ő�X�e�[�^�X�ɐݒ�
	mCharaMaxStatus = ENEMY4_STATUS[index];
	// ���݂̃X�e�[�^�X���ő�l�ɂ��邱�ƂŁAHP��
	mCharaStatus = mCharaMaxStatus;

	//mpHpGauge->SetMaxValue(mCharaMaxStatus.hp);
	//mpHpGauge->SetValue(mCharaStatus.hp);
}

// ��_���[�W����
void CBeholder::TakeDamage(int damage, CObjectBase* causedObj)
{
	//HP����_���[�W������
	if (mCharaStatus.hp -= damage)
	{
		mState = EState::eHit;
	}
	// HP��0�ȉ��ɂȂ�����A
	if (mCharaStatus.hp <= 0)
	{
		// ���S����
		Death();
	}

	if (causedObj != nullptr)
	{
		// �_���[�W��^��������̕����֌���
		CVector dir = causedObj->Position() - Position();
		dir.Y(0.0f);
		dir.Normalize();
		Rotation(CQuaternion::LookRotation(dir));

		// �m�b�N�o�b�N�Ń_���[�W��^��������̕���������ɃY����
		Position(Position() - dir * Scale().X() * 0.4f);
	}
}


// ���S����
void CBeholder::Death()
{
	// ���S��Ԃֈڍs
	mState = EState::eDie;
}