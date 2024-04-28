#include "CTurtle.h"
#include "CPlayer.h"
#include "CHpGauge.h"
#include "CCollisionManager.h"
#include "Maths.h"

// �T�̃C���X�^���X
CTurtle* CTurtle::spInstance = nullptr;

int CTurtle::mHp;

#define ENEMY_HEIGHT 1.0f
#define WITHIN_RANGE 40.0f       // �͈͓�
#define MOVE_SPEED 0.05f         // �ړ����x
#define GRAVITY 0.0625f          // �d��
#define WALK_RANGE 100.0f        // �ǐՂ���͈�
#define STOP_RANGE 24.5f         // �ǐՂ����߂�͈�
#define ROTATE_RANGE  250.0f     // ��]����͈�

// �T�̃A�j���[�V�����f�[�^�̃e�[�u��
const CTurtle::AnimData CTurtle::ANIM_DATA[] =
{
	{ "",										                    true,	 0.0f,	0.0f},  // T�|�[�Y
	{ "Character\\Enemy\\Turtle\\animation\\TurtleIdleNormal.x",	true,	51.0f,	0.5f},  // �ҋ@ 51.0f
	{ "Character\\Enemy\\Turtle\\animation\\TurtleIdleBattle.x",	true,	25.0f,	0.5f},  // �ҋ@2 25.0f
	{ "Character\\Enemy\\Turtle\\animation\\TurtleIdle.x",	        true,	71.0f,	0.5f},  // ���񂷑ҋ@ 71.0f
	{ "Character\\Enemy\\Turtle\\animation\\TurtleIdle2.x",     	true,	61.0f,	0.5f},  // ���񂷑ҋ@2 61.0f
	{ "Character\\Enemy\\Turtle\\animation\\TurtleAttack.x",	    true,	26.0f,	0.5f},	// �U�� 26.0f
	{ "Character\\Enemy\\Turtle\\animation\\TurtleAttack2.x",	true,	52.0f	},	    // �U��2 26.0f
	{ "Character\\Enemy\\Turtle\\animation\\TurtleGetHit.x",	true,	52.0f	},	    // �q�b�g 26.0f
	{ "Character\\Enemy\\Turtle\\animation\\TurtleDefend.x",	false,	36.0f	},	    // �h�� 18.0f
	{ "Character\\Enemy\\Turtle\\animation\\TurtleDefendHit.x",	true,	24.0f	},	    // �h�䒆�̃q�b�g 8.0f
	{ "Character\\Enemy\\Turtle\\animation\\TurtleDefendIdle.x",	true,	24.0f	},	// �h�䒆�̑ҋ@ 8.0f
	{ "Character\\Enemy\\Turtle\\animation\\TurtleDie.x",	true,	122.0f	},	        // ���� 61.0f
	{ "Character\\Enemy\\Turtle\\animation\\TurtleDizzy.x",	true,	82.0f	},	        // �߂܂� 41.0f
	{ "Character\\Enemy\\Turtle\\animation\\TurtleRun.x",	true,	40.0f	},          // ���� 17.0f

};

// �R���X�g���N�^
CTurtle::CTurtle()
	: mState(EState::eIdle)
	, mpRideObject(nullptr)
	, mAttackTime(0)
	, mDefenseTime(0)
	, mIsGrounded(false)
	, mMoveSpeed(CVector::zero)
{
	//�C���X�^���X�̐ݒ�
	spInstance = this;

	// ���f���f�[�^�ǂݍ���
	CModelX* model = CResourceManager::Get<CModelX>("Turtle");

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

	mpColliderLine = new CColliderLine
	(
		this, ELayer::eField,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, ENEMY_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });

	// �L�����N�^�[�����߂�����
	mpColliderSphere = new CColliderSphere
	(
		this, ELayer::eEnemy,
		0.65f, false, 5.0f
	);
	mpColliderSphere->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy});
	mpColliderSphere->Position(0.0f, 0.5f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬
	mpDamageCol = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.65f, false
	);
	//�@�_���[�W���󂯂�R���C�_�[��
	//�@�Փ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageCol->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol->SetCollisionTags({ ETag::eWeapon });
	// �_���[�W���󂯂�R���C�_�[��������ւ��炷
	mpDamageCol->Position(0.0f, 0.3f, -0.1f); 

	// �_���[�W��^����R���C�_�[
	mpAttackCol = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.65f, false
	);
	mpAttackCol->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackCol->SetCollisionTags({ ETag::ePlayer });
	mpAttackCol->Position(0.3f, 0.5f, 0.0f);

	// �U���R���C�_�[���T�̑̂̍s��ɃA�^�b�`
	const CMatrix* bodyMty = GetFrameMtx("Armature_Body");
	mpAttackCol->SetAttachMtx(bodyMty);

	// �ŏ��̍U���R���C�_�[�𖳌��ɂ��Ă���
	mpAttackCol->SetEnable(false);
}

CTurtle::~CTurtle()
{
	SAFE_DELETE(mpColliderLine);
	SAFE_DELETE(mpColliderSphere);
	SAFE_DELETE(mpDamageCol);
	SAFE_DELETE(mpAttackCol);
}

// �C���X�^���X
CTurtle* CTurtle::Instance()
{
	return spInstance;
}

// �A�j���[�V�����؂�ւ�
void CTurtle::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// ��Ԃ̐؂�ւ�
void CTurtle::ChangeState(EState state)
{
	if (mState == state) return;
	mState = state;
}

// �ҋ@���
void CTurtle::UpdateIdle()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eIdle);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle);
	}
}

// �ҋ@���2
void CTurtle::UpdateIdle2()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eIdle2);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle2);
	}

	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();
	if (vectorp >= STOP_RANGE && vectorp <= WALK_RANGE)
	{
		ChangeState(EState::eRun);
	}
	else
	{
		if (IsAnimationFinished())
		{
			ChangeState(EState::eIdle2);
		}
	}
}

// �ҋ@���3
void CTurtle::UpdateIdle3()
{
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eIdle3);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle3);
	}
}

// �U��
void CTurtle::UpdateAttack()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	SetAnimationSpeed(0.5f);
	ChangeAnimation(EAnimType::eAttack);
	AttackStart();
	// �U���I���҂���Ԃֈڍs
	ChangeState(EState::eAttackWait);
}

// �U��2
void CTurtle::UpdateAttack2()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eAttack2);
	AttackStart();
	// �U��2�I���҂���Ԃֈڍs
	ChangeState(EState::eAttackWait);
}

// �U���I���҂�
void CTurtle::UpdateAttackWait()
{
	if (IsAnimationFinished())
	{
		AttackEnd();
		ChangeState(EState::eIdle2);
	}
}

// �q�b�g
void CTurtle::UpdateHit()
{
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
			ChangeState(EState::eDizzy);
		}
		else
		{
			// �v���C���[�̍U�����q�b�g�������̑ҋ@��Ԃֈڍs
			ChangeState(EState::eIdle2);
			ChangeAnimation(EAnimType::eIdle2);
		}
	}
}

// �h��
void CTurtle::UpdateDefense()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eDefense);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eDefenseIdle);
	}
}

// �h�䒆�̃q�b�g
void CTurtle::UpdateDefenseHit()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eDefenseHit);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eDefenseIdle);
	}
}

// �h�䒆�̑ҋ@
void CTurtle::UpdateDefenseIdle()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eDefenseIdle);

	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();
	if (vectorp <= ROTATE_RANGE)
	{
		// �v���C���[�̂�������֌���
		CVector dir = player->Position() - Position();
		dir.Y(0.0f);
		dir.Normalize();
		Rotation(CQuaternion::LookRotation(dir));
	}

	if (IsAnimationFinished())
	{
		if (mDefenseTime >= 800)
		{
			ChangeState(EState::eAttack2);
		}
		else
		{
			ChangeState(EState::eDefenseIdle);
		}
	}
}

// ����
void CTurtle::UpdateDie()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eDie);
	if (IsAnimationFinished())
	{
		Kill();
		// �G�l�~�[�̎��S����
		CEnemy::TurtleDeath();
	}
}

// �߂܂�(����)
void CTurtle::UpdateDizzy()
{
	ChangeAnimation(EAnimType::eDizzy);
	if (IsAnimationFinished())
	{
		// �v���C���[�̍U�����q�b�g�������̑ҋ@��Ԃֈڍs
		ChangeState(EState::eIdle2);
	}
}

// ����
void CTurtle::UpdateRun()
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
	if (vectorp <= STOP_RANGE || vectorp >= WALK_RANGE)
	{
		mMoveSpeed.X(0.0f);
		mMoveSpeed.Z(0.0f);
		ChangeState(EState::eIdle3);
		ChangeAnimation(EAnimType::eIdle4);
	}
}

// �X�V����
void CTurtle::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;
	mHp = mCharaStatus.hp;
	mMoveSpeed.Y(0.0f);

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
		// �U���I���҂�
	case EState::eAttackWait:
		UpdateAttackWait();
		break;
		// �q�b�g
	case EState::eHit:
		UpdateHit();
		break;
		// �h��
	case EState::eDefense:
		UpdateDefense();
		break;
		// �h�䒆�̃q�b�g
	case EState::eDefenseHit:
		UpdateDefenseHit();
		break;
		// �h�䒆�̃q�b�g
	case EState::eDefenseIdle:
		UpdateDefenseIdle();
		break;
		// ����
	case EState::eDie:
		UpdateDie();
		break;
		// �߂܂�(����)
	case EState::eDizzy:
		UpdateDizzy();
		break;
		// ����
	case EState::eRun:
		UpdateRun();
		break;
	}

	// HP�Q�[�W�̍��W���X�V(�G�̍��W�̏�����̍��W)
	CVector gaugePos = Position() + CVector(0.0f, 30.0f, 0.0f);
	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();

	if (vectorp <= WITHIN_RANGE && mState != EState::eIdle2 && mState != EState::eAttack && mState != EState::eAttackWait&& mState != EState::eHit
		&& mState != EState::eDefense && mState != EState::eDefenseHit && mState != EState::eDefenseIdle && mState != EState::eDie && mState != EState::eDizzy
		&& mState != EState::eRun)
	{
		ChangeState(EState::eIdle2);
	}

	if (mState == EState::eRun || mState == EState::eIdle3 || mState == EState::eAttack || mState == EState::eAttack2 ||
		mState == EState::eDefense || mState == EState::eHit || mState == EState::eDizzy || mState == EState::eAttackWait
		|| mState == EState::eDefenseHit || mState == EState::eDefenseIdle)
	{
		mpHpGauge->SetWorldPos(gaugePos);
	}

	if (mState != EState::eDefense || mState == EState::eDefenseIdle)
	{
		if (mState == EState::eIdle2 || mState == EState::eRun || mState == EState::eDefense ||mState == EState::eDefenseIdle
			|| mState == EState::eAttack || mState == EState::eAttack2 || mState == EState::eAttackWait)
		{
			mAttackTime++;

			if (vectorp <= ROTATE_RANGE)
			{
				// �v���C���[�̂�������֌���
				CVector dir = player->Position() - Position();
				dir.Y(0.0f);
				dir.Normalize();
				Rotation(CQuaternion::LookRotation(dir));
			}

			if (mAttackTime > 200)
			{
				// �U��2
				bool Attack2 = false;
				// �h��
				bool Defense = false;
				// �U��2�̊m�����ŏ���2�ő�5
				int probability2 = Math::Rand(2, 4);
				// �h��̊m�����ŏ���5�ő�7
				int probability3 = Math::Rand(4, 6);

				if (probability2 == 2)Attack2 = true;
				if (probability3 == 4)Defense = true;

				if (Attack2)
				{
					ChangeState(EState::eAttack2);
				}
				else if (Defense)
				{
					ChangeState(EState::eDefense);
				}
				else
				{
					ChangeState(EState::eAttack);
				}
			}
			if (mState == EState::eAttack || mState == EState::eAttack2 || mState == EState::eDefense)
			{
				mAttackTime = 0;
			}
		}
	}

	if (vectorp >= STOP_RANGE && vectorp <= WALK_RANGE)
	{
		Position(Position() + mMoveSpeed * MOVE_SPEED);
	}

	if (Position().Y() >= 0.5f)
	{
		mMoveSpeed -= CVector(0.0f, GRAVITY, 0.0f);
	}

	// �h�䎞�Ԍv��
	if (mState == EState::eDefense || mState == EState::eDefenseHit || mState == EState::eDefenseIdle)
	{
		mDefenseTime++;
	}
	else
	{
		mDefenseTime = 0;
	}

	// �L�����N�^�[�̍X�V
	CXCharacter::Update();

	mpAttackCol->Update();

	mIsGrounded = false;

	// HP�Q�[�W�Ɍ��݂�HP��ݒ�
	mpHpGauge->SetValue(mCharaStatus.hp);
	CDebugPrint::Print("HP %d\n", mCharaStatus.hp);
}

// �Փˏ���
void CTurtle::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// �Փ˂��������̃R���C�_�[���U������p�̃R���C�_�[�ł���΁A
	if (self == mpAttackCol && mState != EState::eIdle && mState != EState::eIdle2 &&
		mState != EState::eIdle3)
	{
		// �L�����̃|�C���^�ɕϊ�
		CCharaBase* chara = dynamic_cast<CCharaBase*> (other->Owner());
		// ����̃R���C�_�[�̎����傪�L�����ł���΁A
		if (chara != nullptr)
		{
			// ���ɍU���ς݂̃L�����łȂ����
			if (!IsAttackHitObj(chara))
			{
				int damage = CalcDamage(this, chara);

				// �_���[�W��^����
				chara->TakeDamage(damage, this);

				// �U���ς݃��X�g�ɒǉ�
				AddAttackHitObj(chara);
			}
		}
	}
	else if (self == mpColliderLine)
	{
		if (other->Layer() == ELayer::eField)
		{
			Position(Position() + hit.adjust * hit.weight);
			mIsGrounded = true;
			//mMoveSpeed.Y(0.0f);

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
void CTurtle::AttackStart()
{
	CXCharacter::AttackStart();
	// �U�����n�܂�����A�U������p�̃R���C�_�[���I���ɂ���
	mpAttackCol->SetEnable(true);
}

// �U���I��
void CTurtle::AttackEnd()
{
	CXCharacter::AttackEnd();
	// �U�����I���΁A�U������p�̃R���C�_�[���I�t�ɂ���
	mpAttackCol->SetEnable(false);
}

// �`��
void CTurtle::Render()
{
	CXCharacter::Render();
}

// 1���x���A�b�v
void CTurtle::LevelUp()
{
	int level = mCharaStatus.level;
	ChangeLevel(level + 1);
}

// ���x����ύX
void CTurtle::ChangeLevel(int level)
{
	// �X�e�[�^�X�̃e�[�u���̃C���f�b�N�X�l�ɕϊ�
	int index = Math::Clamp(level - 1, 0, ENEMY__LEVEL_MAX);
	// �ő�X�e�[�^�X�ɐݒ�
	mCharaMaxStatus = ENEMY3_STATUS[index];
	// ���݂̃X�e�[�^�X���ő�l�ɂ��邱�ƂŁAHP��
	mCharaStatus = mCharaMaxStatus;

	mpHpGauge->SetMaxValue(mCharaMaxStatus.hp);
	mpHpGauge->SetValue(mCharaStatus.hp);
}

// ��_���[�W����
void CTurtle::TakeDamage(int damage, CObjectBase* causedObj)
{
	//HP����_���[�W������
	if (mCharaStatus.hp -= damage)
	{
		if (mState == EState::eDefense || mState == EState::eDefenseIdle)
		{
			ChangeState(EState::eDefenseHit);
		}
		else
		{
			ChangeState(EState::eHit);
		}
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
		Position(Position() - dir * Scale().X() * 0.01f);
	}
}

// �h��͂̋����������擾
float CTurtle::GetDefBuff(const CVector& attackDir)const
{
	// �h���Ԃł���΁A�h��2�{
	if (mState == EState::eDefense|| mState == EState::eDefenseIdle) return 2.0f;

	// �ʏ펞�̖h��̊���
	return mBaseDefenseBuffRatio;
}


// ���S����
void CTurtle::Death()
{
	// ���S��Ԃֈڍs
	ChangeState(EState::eDie);
}