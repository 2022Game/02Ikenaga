#include "CDragon.h"
#include "CPlayer.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "CInput.h"
#include "Maths.h"

// �h���S���̃C���X�^���X
CDragon* CDragon::spInstance = nullptr;

#define ENEMY_HEIGHT 1.0f
#define WITHIN_RANGE 40.0f    // �͈͓�

// �h���S���̃A�j���[�V�����f�[�^�̃e�[�u��
const CDragon::AnimData CDragon::ANIM_DATA[] =
{
	{ "",										true,	0.0f	},// T�|�[�Y
	//{ "Character\\Enemy\\Dragon\\animation\\DragonIdle.x",	true,	82.0f	},	    // �ҋ@ 41.0f
	//{ "Character\\Enemy\\Dragon\\animation\\DragonIdle2.x",	true,	202.0f	},	    // �ҋ@ 101.0f
	//{ "Character\\Enemy\\Dragon\\animation\\DragonAttack.x",	true,	162.0f	},	    // �U�� 81.0f
	//{ "Character\\Enemy\\Dragon\\animation\\DragonAttack2.x",	true,	182.0f	},	    // �U��2 91.0f
	//{ "Character\\Enemy\\Dragon\\animation\\DragonAttack3.x",	true,	72.0f	},	    // �U��3 36.0f
	//{ "Character\\Enemy\\Dragon\\animation\\DragonDefend.x",	true,	122.0f	},	    // �h�� 61.0f
	//{ "Character\\Enemy\\Dragon\\animation\\DragonDie.x",	true,	130.0f	},	        // ���� 65.0f
	//{ "Character\\Enemy\\Dragon\\animation\\DragonFlyFlame.x",	true,	182.0f	},	// �t���C�t���[�� 91.0f
	//{ "Character\\Enemy\\Dragon\\animation\\DragonFlyForward.x",	true,	62.0f	},	// �t���C�t�H���[�h 31.0f
	//{ "Character\\Enemy\\Dragon\\animation\\DragonFlyGlide.x",	true,	51.0f	},	// �t���C�O���C�h 51.0f
	//{ "Character\\Enemy\\Dragon\\animation\\DragonFlyIdle.x",	true,	62.0f	},	    // �t���C�A�C�h�� 31.0f
	//{ "Character\\Enemy\\Dragon\\animation\\DragonGetHit.x",	true,	82.0f	},	    // �t���C�A�C�h�� 41.0f
	//{ "Character\\Enemy\\Dragon\\animation\\DragonLand.x",	true,	242.0f	},	    // ���n 121.0f
	//{ "Character\\Enemy\\Dragon\\animation\\DragonRun.x",	true,	42.0f	},	        // ���� 21.0f
	//{ "Character\\Enemy\\Dragon\\animation\\DragonScream.x",	true,	202.0f	},	    // �Y���� 101.0f
	{ "Character\\Enemy\\Dragon\\animation\\DragonSleep.x",	true,	162.0f	},	    // �Q�� 81.0f
	//{ "Character\\Enemy\\Dragon\\animation\\DragonTakeOff.x",	true,	242.0f	},	    // ��s�J�n����O 121.0f
	//{ "Character\\Enemy\\Dragon\\animation\\DragonWalk.x",	true,	82.0f	},	        // ���� 41.0f
};

// �R���X�g���N�^
CDragon::CDragon()
	: mpRideObject(nullptr)
	, mAttackTime(0)
{
	//�C���X�^���X�̐ݒ�
	spInstance = this;

	// ���f���f�[�^�ǂݍ���
	CModelX* model = CResourceManager::Get<CModelX>("Dragon");

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

CDragon::~CDragon()
{
	/*SAFE_DELETE(mpColliderLine);
	SAFE_DELETE(mpColliderSphere);
	SAFE_DELETE(mpDamageCol);
	SAFE_DELETE(mpAttackCol);*/
}

CDragon* CDragon::Instance()
{
	return spInstance;
}

// �A�j���[�V�����؂�ւ�
void CDragon::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// �키�O�̑ҋ@���
void CDragon::UpdateIdle()
{
	if (IsAnimationFinished())
	{
		mState = EState::eIdle2;
	}
}

// �키�O�̑ҋ@���2
void CDragon::UpdateIdle2()
{
	ChangeAnimation(EAnimType::eIdle2);
	if (IsAnimationFinished())
	{
		mState = EState::eIdle3;
	}
}

// �ҋ@���3
void CDragon::UpdateIdle3()
{
	ChangeAnimation(EAnimType::eIdle3);
	if (IsAnimationFinished())
	{
		mState = EState::eIdle3;
	}
}

// �U��
void CDragon::UpdateAttack()
{
	ChangeAnimation(EAnimType::eAttack);
	AttackStart();
	// �U��2�I���҂���Ԃֈڍs
	mState = EState::eAttackWait;
}

// �U��2
void CDragon::UpdateAttack2()
{
	ChangeAnimation(EAnimType::eAttack2);
	AttackStart();
	// �U��2�I���҂���Ԃֈڍs
	mState = EState::eAttackWait;
}


// �U��3
void CDragon::UpdateAttack3()
{
	ChangeAnimation(EAnimType::eAttack3);
	AttackStart();
	// �U��2�I���҂���Ԃֈڍs
	mState = EState::eAttackWait;
}

// �U���I���҂�
void CDragon::UpdateAttackWait()
{
	if (IsAnimationFinished())
	{
		AttackEnd();
		mState = EState::eIdle3;
	}
}

// �q�b�g
void CDragon::UpdateHit()
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
			mState = EState::eIdle3;
			ChangeAnimation(EAnimType::eIdle3);
		}
	}
}

// ����
void CDragon::UpdateDie()
{
	//ChangeAnimation(EAnimType::eDie);
}

// �߂܂�(����)
void CDragon::UpdateDizzy()
{
	//ChangeAnimation(EAnimType::eDizzy);
}

// �X�V����
void CDragon::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;

	// ��Ԃɍ��킹�āA�X�V������؂�ւ���
	switch (mState)
	{
		// �키�O�̑ҋ@���
	case EState::eIdle:
		UpdateIdle();
		break;
		// �키�O�̑ҋ@���2
	case EState::eIdle2:
		UpdateIdle2();
		break;
		// �ҋ@���3
	case EState::eIdle3:
		UpdateIdle3();
		break;
		// �U��
	case EState::eAttack:
		UpdateAttack();
		break;
		// �U��2
	case EState::eAttack2:
		UpdateAttack2();
		break;
		// �U��3
	case EState::eAttack3:
		UpdateAttack3();
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
	if (vectorp <= WITHIN_RANGE && mState != EState::eIdle3)
	{
		//UpdateIdle();
	}
	if (mState == EState::eIdle3)
	{
		//mAttackTime++;
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
			//	mState = EState::eAttack;
			//}
		}
		if (mState == EState::eAttack || mState == EState::eAttack2 || mState == EState::eAttack3)
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

	if (CInput::PushKey('Q'))
	{
		//mState = EState::eAttack;
	}
}

// �Փˏ���
void CDragon::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
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
void CDragon::AttackStart()
{
	//CXCharacter::AttackStart();
	// �U�����n�܂�����A�U������p�̃R���C�_�[���I���ɂ���
	//mpAttackCol->SetEnable(true);
}

// �U���I��
void CDragon::AttackEnd()
{
	//CXCharacter::AttackEnd();
	// �U�����I���΁A�U������p�̃R���C�_�[���I�t�ɂ���
	//mpAttackCol->SetEnable(false);
}

// �`��
void CDragon::Render()
{
	CXCharacter::Render();
}

// 1���x���A�b�v
void CDragon::LevelUp()
{
	int level = mCharaStatus.level;
	ChangeLevel(level + 1);
}

// ���x����ύX
void CDragon::ChangeLevel(int level)
{
	// �X�e�[�^�X�̃e�[�u���̃C���f�b�N�X�l�ɕϊ�
	int index = Math::Clamp(level - 1, 0, ENEMY__LEVEL_MAX);
	// �ő�X�e�[�^�X�ɐݒ�
	mCharaMaxStatus = ENEMY2_STATUS[index];
	// ���݂̃X�e�[�^�X���ő�l�ɂ��邱�ƂŁAHP��
	mCharaStatus = mCharaMaxStatus;

	//mpHpGauge->SetMaxValue(mCharaMaxStatus.hp);
	//mpHpGauge->SetValue(mCharaStatus.hp);
}

// ��_���[�W����
void CDragon::TakeDamage(int damage, CObjectBase* causedObj)
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
void CDragon::Death()
{
	// ���S��Ԃֈڍs
	mState = EState::eDie;
}