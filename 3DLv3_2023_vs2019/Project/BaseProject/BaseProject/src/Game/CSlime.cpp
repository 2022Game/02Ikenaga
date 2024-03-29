#include "CSlime.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "CInput.h"
#include "CHpGauge.h"
#include "Maths.h"
#include "CPlayer.h"
#include "CCamera.h"

// ���b�h�X���C��(�G�l�~�[)�̃C���X�^���X
CSlime* CSlime::spInstance = nullptr;

#define ENEMY_HEIGHT 1.0f
#define MOVE_SPEED 0.07f    // �ړ����x
#define GRAVITY 0.0625f    // �d��

#define WALK_RANGE 100.0f        // �ǐՂ���͈�
#define STOP_RANGE 24.5f         // �ǐՂ����߂�͈�
#define ROTATE_RANGE  250.0f     // ��]����͈�

// ���b�h�X���C��(�G�l�~�[)�̃A�j���[�V�����f�[�^�̃e�[�u��
const CSlime::AnimData CSlime::ANIM_DATA[] =
{
	{ "",										true,	0.0f	},	// T�|�[�Y
	{ "Character\\Enemy\\Slime\\animation\\SlimeIdleNormal.x",	true,	70.0f	},  // �A�C�h���ʏ� 51.0f
	{ "Character\\Enemy\\Slime\\animation\\SlimeSenseSomethingStart.x",	true,	120.0f	},  // �J�n�̌��� 63.0f
	{ "Character\\Enemy\\Slime\\animation\\SlimeSenseSomethingRoutine.x",	true,	140.0f	},  // ���� 71.0f
	{ "Character\\Enemy\\Slime\\animation\\SlimeIdleBattle.x",	true,	50.0f	},  // �A�C�h���o�g�� 25.0f
	{ "Character\\Enemy\\Slime\\animation\\SlimeAttack.x",	true,	52.0f	},  // �U�� 26.0f
	{ "Character\\Enemy\\Slime\\animation\\SlimeAttack2.x",	true,	80.0f	},  // �U��2 26.0f
	{ "Character\\Enemy\\Slime\\animation\\SlimeGetHit.x",	true,	70.0f	},  // �q�b�g 26.0f
	{ "Character\\Enemy\\Slime\\animation\\SlimeDie.x",	true,	90.0f	},  // ���� 41.0f
	{ "Character\\Enemy\\Slime\\animation\\SlimeDizzy.x",	true,	100.0f	},  // �߂܂� 41.0f
	{ "Character\\Enemy\\Slime\\animation\\SlimeRun.x",	true,	30.0f	},  // ���� 21.0f
	{ "Character\\Enemy\\Slime\\animation\\SlimeWalk.x",	true,	50.0f	},  // ���� 31.0f
//	{ "Character\\Enemy\\Slime\\animation\\SlimeWalkRight.x",	true,	31.0f	},  // �E�Ɉړ�
	//{ "Character\\Enemy\\Slime\\animation\\SlimeWalkLeft.x",	true,	31.0f	},  // ���Ɉړ�
	//{ "Character\\Enemy\\Slime\\animation\\SlimeTaunt.x",	true,	21.0f	},  // ����
	//{ "Character\\Enemy\\Slime\\animation\\SlimeVictory.x",	true,	81.0f	},  // ����
	//{ "Character\\Enemy\\Slime\\animation\\SlimeWalkBack.x",	true,	31.0f	},  // ���ɕ���
};

bool CSlime::IsDeath() const
{
	return mCharaStatus.hp <= 0;
}

int CSlime::mHp;

// �R���X�g���N�^
CSlime::CSlime()
	:mState(EState::eIdle)
	, mpRideObject(nullptr)
	,mAttackTime(0)
{
	// �C���X�^���X�̐ݒ�
	spInstance = this;

	// ���f���f�[�^�ǂݍ���
	CModelX* model = CResourceManager::Get<CModelX>("Slime");

	// �ŏ���1���x���ɐݒ�
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

	mpColliderLine = new CColliderLine
	(
		this, ELayer::eEnemy,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, ENEMY_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });

	// �L�����N�^�[�����߂�����
	mpColliderSphere = new CColliderSphere
	(
		this,ELayer::eEnemy,
		0.5f,false,5.0f
	);
	mpColliderSphere->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphere->Position(0.0f, 0.3f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬
	mpDamageCol = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.4f,false
	);
	//�@�_���[�W���󂯂�R���C�_�[��
	//�@�Փ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageCol->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol->SetCollisionTags({ ETag::eWeapon });
	//�_���[�W���󂯂�R���C�_�[��������ւ��炷
	mpDamageCol->Position(0.0f, 0.3f,0.0f);

	// �_���[�W��^����R���C�_�[
	mpAttackCol = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.5f,false
	);
	mpAttackCol->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackCol->SetCollisionTags({ ETag::ePlayer });
	mpAttackCol->Position(0.0f, 0.3f, 0.0f);
	
	// �U���R���C�_�[���X���C����Body�̍s��ɃA�^�b�`
	const CMatrix* bodyMty = GetFrameMtx("Armature_Body");
	mpAttackCol->SetAttachMtx(bodyMty);

	// �ŏ��̍U���R���C�_�[�𖳌��ɂ��Ă���
	mpAttackCol->SetEnable(false);
}

CSlime::~CSlime()
{
	SAFE_DELETE(mpColliderLine);
	SAFE_DELETE(mpColliderSphere);
	// �_���[�W���󂯂�R���C�_�[���폜
	SAFE_DELETE(mpDamageCol);
	SAFE_DELETE(mpAttackCol);
}

CSlime* CSlime::Instance()
{
	return spInstance;
}

// �A�j���[�V�����؂�ւ�
void CSlime::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// �ҋ@���
void CSlime::UpdateIdle()
{
	if (IsAnimationFinished())
	{
		mState = EState::eIdle2;
	}
}

// �ҋ@2���
void CSlime::UpdateIdle2()
{
	ChangeAnimation(EAnimType::eIdle2);
	if (IsAnimationFinished())
	{
		mState = EState::eIdleWait;
	}
}

// �U���������̑ҋ@���
void CSlime::UpdateIdle3()
{
	ChangeAnimation(EAnimType::eIdle4);
	if (IsAnimationFinished())
	{
		mState = EState::eIdle3;
	}
	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();
	if ( vectorp >=STOP_RANGE && vectorp <= WALK_RANGE)
	{
		mState = EState::eRun;
	}
	else
	{
		ChangeAnimation(EAnimType::eIdle4);
		if (IsAnimationFinished())
		{
			mState = EState::eIdle3;
		}
	}
}

// �ҋ@2�̏I���҂�
void CSlime::UpdateIdleWait()
{
	// �ҋ@3�A�j���[�V�����ɐ؂�ւ�
	ChangeAnimation(EAnimType::eIdle3);
	// �ҋ@3�A�j���[�V�������I��������A
	if (IsAnimationFinished())
	{
		// �ҋ@��Ԃֈڍs
		mState = EState::eIdle;
		ChangeAnimation(EAnimType::eIdle);
	}
}

// �U��
void CSlime::UpdateAttack()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	// �U���A�j���[�V�������J�n
	ChangeAnimation(EAnimType::eAttack);
	AttackStart();
	// �U���I���҂���Ԃֈڍs
	mState = EState::eAttackWait;
}

// �U��2
void CSlime::UpdateAttack2()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	// �U��2�A�j���[�V�������J�n
	ChangeAnimation(EAnimType::eAttack2);
	if (mAnimationFrame >= 35.0f)
	{
		// �U��2�I���҂���Ԃֈڍs
		mState = EState::eAttackWait;
		AttackStart();
	}
}

// �U���ƍU��2�I���҂�
void CSlime::UpdateAttackWait()
{
	// �U���ƍU��2�A�j���[�V�������I��������A
	if (IsAnimationFinished())
	{
		AttackEnd();
		CPlayer* player = CPlayer::Instance();
		float vectorp = (player->Position() - Position()).Length();
		if (vectorp >=STOP_RANGE && vectorp <= WALK_RANGE)
		{
			mState = EState::eRun;
		}
		else
		{
			// �v���C���[�̍U�����q�b�g�������̑ҋ@��Ԃֈڍs
			mState = EState::eIdle3;
			ChangeAnimation(EAnimType::eIdle4);
		}
	}
}

// �q�b�g
void CSlime::UpdateHit()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);

	// �q�b�g�A�j���[�V�������J�n
	ChangeAnimation(EAnimType::eHit);
	if (IsAnimationFinished())
	{
		// �߂܂���false�ɂ���
		bool stan = false;
		// �m�����ŏ���0�ő�20
		int probability = Math::Rand(0, 20);
		if (probability == 1)stan = true;
		if (stan)
		{
			mState = EState::eDizzy;
		}
		else
		{
			// �v���C���[�̍U�����q�b�g�������̑ҋ@��Ԃֈڍs
			mState = EState::eIdle3;
			ChangeAnimation(EAnimType::eIdle4);
		}
	}
}

// ���ʎ�
void CSlime::UpdateDie()
{
	// ���ʎ��̃A�j���[�V�������J�n
	ChangeAnimation(EAnimType::eDie);
	if (IsAnimationFinished())
	{
		Kill();
		// �G�l�~�[�̎��S����
		CEnemy::Death();
	}
}

// �߂܂�(����)
void CSlime::UpdateDizzy()
{
	// �߂܂�(����)�A�j���[�V�������J�n
	ChangeAnimation(EAnimType::eDizzy);
	if (IsAnimationFinished())
	{
		// �v���C���[�̍U�����q�b�g�������̑ҋ@��Ԃֈڍs
		mState = EState::eIdle3;
		ChangeAnimation(EAnimType::eIdle4);
	}
}

// ����
void CSlime::UpdateRun() 
{
	ChangeAnimation(EAnimType::eRun);

	CPlayer* player = CPlayer::Instance();
	CVector nowPos = (player->Position() - Position()).Normalized();
	float vectorp = (player->Position() - Position()).Length();
	// �ǐՂ���߂Ď~�܂�
	if (vectorp <= 22.0f && vectorp >= 24.0f)
	{
		mMoveSpeed.X(0.0f);
		mMoveSpeed.Z(0.0f);

		// ��]����͈͂ł����
		if (vectorp <= ROTATE_RANGE)
		{
			// �v���C���[�̂�������֌���
			CVector dir = player->Position() - Position();
			dir.Y(0.0f);
			dir.Normalize();
			Rotation(CQuaternion::LookRotation(dir));

			mMoveSpeed.X(0.0f);
			mMoveSpeed.Z(0.0f);
		}
	}
	// �͈͓��̎��A�ړ����ǐՂ���
	else if (vectorp >= 24.0f && vectorp <= WALK_RANGE)
	{
		mMoveSpeed += nowPos * MOVE_SPEED;
	}
	// �ǐՂ��~�܂������A�U���p�̑ҋ@���[�V������
	if (vectorp <= STOP_RANGE ||vectorp >= WALK_RANGE)
	{
		mMoveSpeed.X(0.0f);
		mMoveSpeed.Z(0.0f);
		mState = EState::eIdle3;
		ChangeAnimation(EAnimType::eIdle4);
	}
}



// �X�V����
void CSlime::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;
	mHp = mCharaStatus.hp;
	
	// ��Ԃɍ��킹�āA�X�V������؂�ւ���
	switch (mState)
	{
		// �ҋ@���
	case EState::eIdle:
		UpdateIdle();
		break;
	case EState::eIdle2:
		UpdateIdle2();
		break;
	case EState::eIdle3:
		UpdateIdle3();
		break;
		// �ҋ@�I���҂�
	case EState::eIdleWait:
		UpdateIdleWait();
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
		// ���ʎ�
	case EState::eDie:
		UpdateDie();
		break;
		// �߂܂�(����)
	case EState::eDizzy:
		UpdateDizzy();
		break;
		// ���s
	case EState::eRun:
		UpdateRun();
		break;
	}

	// HP�Q�[�W�̍��W���X�V(�G�̍��W�̏�����̍��W)
	CVector gaugePos = Position() + CVector(0.0f, 30.0f, 0.0f);
	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();

	//int rand = 0;
	//rand = Math::Rand(0, 700);

	// HP����������U���J�n
	if (mCharaStatus.hp < mCharaMaxStatus.hp)
	{
		mpHpGauge->SetWorldPos(gaugePos);

		(mAttackTime++ / 100);

		if (mAttackTime > 230)
		{
			// ��U��
			bool BigAttack = false;
			// �m�����ŏ���8�ő�10
			int probability2 = Math::Rand(8, 11);
			if (probability2 == 8)BigAttack = true;

			if (BigAttack)
			{
				mState = EState::eAttack2;
			}
			else
			{
				mState = EState::eAttack;
			}
		}
		else if (mCharaStatus.hp <= 0)
		{
			mAttackTime = 0;
		}
		if (mState == EState::eAttack || mState == EState::eAttack2 || mState == EState::eDizzy)
		{
			mAttackTime = 0;
		}
		CPlayer* player = CPlayer::Instance();
		float vectorp = (player->Position() - Position()).Length();
		if (vectorp >= WALK_RANGE)
		{
			mAttackTime = 0;
		}
	}

	if (mState != EState::eIdle && mState != EState::eIdle2 && mState != EState::eIdleWait)
	{
		if (vectorp <= ROTATE_RANGE)
		{
			// �v���C���[�̂�������֌���
			CVector dir = player->Position() - Position();
			dir.Y(0.0f);
			dir.Normalize();
			Rotation(CQuaternion::LookRotation(dir));
		}
	}
	if (vectorp >= STOP_RANGE && vectorp <= WALK_RANGE)
	{
		Position(Position() + mMoveSpeed * MOVE_SPEED);
		mMoveSpeed -= CVector(0.0f, GRAVITY, 0.0f);
	}

	// �L�����N�^�[�̍X�V
	CXCharacter::Update();

	mpAttackCol->Update();

	mIsGrounded = false;

	// HP�Q�[�W�Ɍ��݂�HP��ݒ�
	mpHpGauge->SetValue(mCharaStatus.hp);

	//CDebugPrint::Print("�U�� %d", mAttackTime);
}

// �Փˏ���
void CSlime::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
    // �Փ˂��������̃R���C�_�[���U������p�̃R���C�_�[�ł���΁A
	if (self == mpAttackCol && mState != EState::eIdle && mState != EState::eIdle2 &&
		mState != EState::eIdle3 && mState != EState::eIdleWait)
	{
		// �L�����̃|�C���^�ɕϊ�
		CCharaBase* chara = dynamic_cast<CCharaBase*> (other->Owner());
		// ����̃R���C�_�[�̎����傪�L�����ł���΁A
		if (chara != nullptr)
		{
			// ���ɍU���ς݂̃L�����łȂ����
			if (!IsAttackHitObj(chara))
			{
				int damage = CalcDamage(this,chara);

				// �_���[�W��^����
				chara->TakeDamage(damage, this);

				// �U���ς݃��X�g�ɒǉ�
				AddAttackHitObj(chara);
			}
		}
	}
	// �t�B�[���h�Ƃ̐ڒn����
	else if (self == mpColliderLine)
	{
		if (other->Layer() == ELayer::eField)
		{
			Position(Position() + hit.adjust * hit.weight);
			mIsGrounded = true;
			mMoveSpeed.Y(0.0f);

			if (other->Tag() == ETag::eRideableObject)
			{
				mpRideObject = other->Owner();
			}
		}
	}
	// �L�����N�^�[���m�̏Փˏ���
	else if (self == mpColliderSphere)
	{
		CVector pushBack = hit.adjust * hit.weight;
		pushBack.Y(0.0f);
		Position(Position() + pushBack);
	}
}

// �U���J�n
void CSlime::AttackStart()
{
	CXCharacter::AttackStart();
	// �U�����n�܂�����A�U������p�̃R���C�_�[���I���ɂ���
	mpAttackCol->SetEnable(true);
}

// �U���I��
void CSlime::AttackEnd()
{
	CXCharacter::AttackEnd();
	// �U�����I���΁A�U������p�̃R���C�_�[���I�t�ɂ���
	mpAttackCol->SetEnable(false);
}

// �`��
void CSlime::Render()
{
	CXCharacter::Render();
}

// 1���x���A�b�v
void CSlime::LevelUp()
{
	int level = mCharaStatus.level;
	ChangeLevel(level + 1);
}

// ���x����ύX
void CSlime::ChangeLevel(int level)
{
	// �X�e�[�^�X�̃e�[�u���̃C���f�b�N�X�l�ɕϊ�
	int index = Math::Clamp(level - 1, 0, ENEMY__LEVEL_MAX);
	// �ő�X�e�[�^�X�ɐݒ�
	mCharaMaxStatus = ENEMY_STATUS[index];
	// ���݂̃X�e�[�^�X���ő�l�ɂ��邱�ƂŁAHP��
	mCharaStatus = mCharaMaxStatus;

	mpHpGauge->SetMaxValue(mCharaMaxStatus.hp);
	mpHpGauge->SetValue(mCharaStatus.hp);
}

// ��_���[�W����
void CSlime::TakeDamage(int damage, CObjectBase* causedObj)
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
void CSlime::Death()
{
	// ���S��Ԃֈڍs
	mState = EState::eDie;
}