#include "CSlime2.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "CInput.h"
#include "CHpGauge.h"
#include "Maths.h"
#include "CExp.h"

// �I�����W�X���C��(�G�l�~�[)�̃C���X�^���X
CSlime2* CSlime2::spInstance = nullptr;

#define MODEL_SLIME "Character\\Enemy\\Slime\\Slime.x"
#define MODEL_SLIME_BLUE "Character\\Enemy\\Slime\\SlimeBlue.x"
#define MODEL_SLIME_ORANGE "Character\\Enemy\\Slime\\SlimeOrange.x"

#define ENEMY_HEIGHT 1.0f
#define GRAVITY 0.0625f

// �I�����W�X���C��(�G�l�~�[)�̃A�j���[�V�����f�[�^�̃e�[�u��
const CSlime2::AnimData CSlime2::ANIM_DATA[] =
{
	{ "",										true,	0.0f	},	// T�|�[�Y
	{ "Character\\Enemy\\Slime\\animation\\SlimeIdleNormal.x",	true,	80.0f	},  // �A�C�h���ʏ� 51.0f
	{ "Character\\Enemy\\Slime\\animation\\SlimeSenseSomethingStart.x",	true,	130.0f	},  // �J�n�̌��� 63.0f
	{ "Character\\Enemy\\Slime\\animation\\SlimeSenseSomethingRoutine.x",	true,	150.0f	},  // ���� 71.0f
	{ "Character\\Enemy\\Slime\\animation\\SlimeIdleBattle.x",	true,	25.0f	},  // �A�C�h���o�g�� 25.0f
	{ "Character\\Enemy\\Slime\\animation\\SlimeAttack.x",	true,	26.0f	},  // �U�� 26.0f
	{ "Character\\Enemy\\Slime\\animation\\SlimeAttack2.x",	true,	70.0f	},  // �U��2 26.0f
	{ "Character\\Enemy\\Slime\\animation\\SlimeGetHit.x",	true,	65.0f	},  // �q�b�g 26.0f
	{ "Character\\Enemy\\Slime\\animation\\SlimeDie.x",	true,	81.0f	},  // ���� 41.0f
	{ "Character\\Enemy\\Slime\\animation\\SlimeDizzy.x",	true,	100.0f	},  // �߂܂� 41.0f
	//{ "Character\\Enemy\\Slime\\animation\\SlimeRun.x",	true,	21.0f	},  // ����
	//{ "Character\\Enemy\\Slime\\animation\\SlimeTaunt.x",	true,	21.0f	},  // ����
	//{ "Character\\Enemy\\Slime\\animation\\SlimeVictory.x",	true,	81.0f	},  // ����
	//{ "Character\\Enemy\\Slime\\animation\\SlimeWalk.x",	true,	31.0f	},  // ����
	//{ "Character\\Enemy\\Slime\\animation\\SlimeWalkBack.x",	true,	31.0f	},  // ���ɕ���
	//{ "Character\\Enemy\\Slime\\animation\\SlimeWalkLeft.x",	true,	31.0f	},  // ���Ɉړ�
	//{ "Character\\Enemy\\Slime\\animation\\SlimeWalkRight.x",	true,	31.0f	},  // �E�Ɉړ�
};

bool CSlime2::IsDeath() const
{
	return mCharaStatus.hp <= 0;
}

int CSlime2::mHp;

// �R���X�g���N�^
CSlime2::CSlime2()
	:mState(EState::eIdle)
	, mpRideObject(nullptr)
	, mAttackTime(0)
{
	//�C���X�^���X�̐ݒ�
	spInstance = this;

	// ���f���f�[�^�ǂݍ���
	CModelX* model = new CModelX();
	model->Load(MODEL_SLIME_ORANGE);

	//mpHpGauge = new CHpGauge();
	//mpHpGauge->SetPos(10.0f, 90.f);

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
		this, ELayer::eEnemy2,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, ENEMY_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });

	// �L�����N�^�[�����߂�����
	mpColliderSphere = new CColliderSphere
	(
		this, ELayer::eEnemy2,
		0.5f, false, 5.0f
	);
	mpColliderSphere->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy });
	mpColliderSphere->Position(0.0f, 0.3f, 0.0f);

	// �_���[�W���󂯂�R���C�_�[���쐬
	mpDamageCol = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.4f, false
	);
	//�@�_���[�W���󂯂�R���C�_�[��
	//�@�Փ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageCol->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol->SetCollisionTags({ ETag::eWeapon });
	//�_���[�W���󂯂�R���C�_�[��������ւ��炷
	mpDamageCol->Position(0.0f, 0.3f, 0.0f);

	// �_���[�W��^����R���C�_�[
	mpAttackCol = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.5f, false
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

CSlime2::~CSlime2()
{
	SAFE_DELETE(mpColliderLine);
	SAFE_DELETE(mpColliderSphere);
	// �_���[�W���󂯂�R���C�_�[���폜
	SAFE_DELETE(mpDamageCol);
	SAFE_DELETE(mpAttackCol);
	SAFE_DELETE(mpModel);
}

CSlime2* CSlime2::Instance()
{
	return spInstance;
}

// �A�j���[�V�����؂�ւ�
void CSlime2::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// �ҋ@���
void CSlime2::UpdateIdle()
{
	if (IsAnimationFinished())
	{
		mState = EState::eIdle2;
	}
}

// �ҋ@2���
void CSlime2::UpdateIdle2()
{
	ChangeAnimation(EAnimType::eIdle2);
	if (IsAnimationFinished())
	{
		mState = EState::eIdleWait;
	}
}

// �U���������̑ҋ@���
void CSlime2::UpdateIdle3()
{
	ChangeAnimation(EAnimType::eIdle4);
	if (IsAnimationFinished())
	{
		mState = EState::eIdle3;
	}
}

// �ҋ@2�̏I���҂�
void CSlime2::UpdateIdleWait()
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
void CSlime2::UpdateAttack()
{
	// �U���A�j���[�V�������J�n
	ChangeAnimation(EAnimType::eAttack);
	// �U���I���҂���Ԃֈڍs
	mState = EState::eAttackWait;
	AttackStart();
}

// �U��2
void CSlime2::UpdateAttack2()
{
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
void CSlime2::UpdateAttackWait()
{
	// �U���ƍU��2�A�j���[�V�������I��������A
	if (IsAnimationFinished())
	{
		// �v���C���[�̍U�����q�b�g�������̑ҋ@��Ԃֈڍs
		mState = EState::eIdle3;
		ChangeAnimation(EAnimType::eIdle4);

		AttackEnd();
	}
}

// �q�b�g
void CSlime2::UpdateHIt()
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
void CSlime2::UpdateDie()
{
	// ���ʎ��̃A�j���[�V�������J�n
	ChangeAnimation(EAnimType::eDie);
	if (IsAnimationFinished())
	{
		Kill();
		/*CVector posY = Position();
		posY.Y(4.5f);
		CExp* exp = new CExp();
		exp->Position(posY);
		exp->Scale(4.0f, 4.0f, 4.0f);*/
	}
}

// �߂܂�(����)
void CSlime2::UpdateDizzy()
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

// �X�V����
void CSlime2::Update()
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
		UpdateHIt();
		break;
		// ���ʎ�
	case EState::eDie:
		UpdateDie();
		break;
		// �߂܂�(����)
	case EState::eDizzy:
		UpdateDizzy();
		break;
	}
	mMoveSpeed -= CVector(0.0f, GRAVITY, 0.0f);
	Position(Position() + mMoveSpeed);

	if (mCharaStatus.hp <= 0)
	{
		// ���S����
		mState = EState::eDie;
	}

	if (mCharaStatus.hp < mCharaMaxStatus.hp)
	{
		mAttackTime++;
		if (mAttackTime > 200)
		{
			// ��U��
			bool BigAttack = false;
			// �m�����ŏ���8�ő�10
			int probability2 = Math::Rand(8, 10);
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
	}

	// �L�����N�^�[�̍X�V
	CXCharacter::Update();

	mIsGrounded = false;
	//mpHpGauge->SetValue(mCharaStatus2.hp);
}

// �Փˏ���
void CSlime2::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
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
				int damage = 0;
				damage = mCharaStatus.power;

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
void CSlime2::AttackStart()
{
	CXCharacter::AttackStart();
	// �U�����n�܂�����A�U������p�̃R���C�_�[���I���ɂ���
	mpAttackCol->SetEnable(true);
}

// �U���I��
void CSlime2::AttackEnd()
{
	CXCharacter::AttackEnd();
	// �U�����I���΁A�U������p�̃R���C�_�[���I�t�ɂ���
	mpAttackCol->SetEnable(false);
}

// �`��
void CSlime2::Render()
{
	CXCharacter::Render();
}

// 1���x���A�b�v
void CSlime2::LevelUp()
{
	int level = mCharaStatus.level;
	ChangeLevel(level + 1);
}

// ���x����ύX
void CSlime2::ChangeLevel(int level)
{
	// �X�e�[�^�X�̃e�[�u���̃C���f�b�N�X�l�ɕϊ�
	int index = Math::Clamp(level - 1, 0, ENEMY__LEVEL_MAX);
	// �ő�X�e�[�^�X�ɐݒ�
	mCharaMaxStatus = ENEMY_STATUS[index];
	// ���݂̃X�e�[�^�X���ő�l�ɂ��邱�ƂŁAHP��
	mCharaStatus = mCharaMaxStatus;
}

// ��_���[�W����
void CSlime2::TakeDamage(int damage, CObjectBase* causedObj)
{
	//���S���Ă�����A�_���[�W�͎󂯂Ȃ�
	//if (mCharaStatus.hp <= 0)return;

	//HP����_���[�W������
	//mCharaStatus.hp = max(mCharaStatus.hp - damage, 0);
	//mCharaStatus.hp -= damage;
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

		// �m�b�N�o�b�N�Ń_���[�W��^��������̕�������
		// ���ɃY����
		Position(Position() - dir * Scale().X() * 0.4f);
	}
}

// ���S����
void CSlime2::Death()
{
	// �G�l�~�[�̎��S����
	CEnemy::Death();
	// ���S��Ԃֈڍs
	mState = EState::eDie;
}