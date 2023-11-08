#include "CEnemy.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "CInput.h"
#include "CHpGauge.h"
#include "Maths.h"

// �G�l�~�[�̃C���X�^���X
CEnemy* CEnemy::spInstance = nullptr;

#define MODEL_SLIME "Character\\Slime\\Slime.x"

#define PLAYER_HEIGHT 1.0f

// �G�l�~�[�̃A�j���[�V�����f�[�^�̃e�[�u��
const CEnemy::AnimData CEnemy::ANIM_DATA[] =
{
	{ "",										true,	0.0f	},	// T�|�[�Y
	{ "Character\\Slime\\animation\\SlimeIdleNormal.x",	true,	70.0f	},  //�A�C�h���ʏ� 51.0f
	{ "Character\\Slime\\animation\\SlimeSenseSomethingStart.x",	true,	120.0f	},  //�J�n�̌��� 63.0f
	{ "Character\\Slime\\animation\\SlimeSenseSomethingRoutine.x",	true,	140.0f	},  //���� 71.0f
	{ "Character\\Slime\\animation\\SlimeIdleBattle.x",	true,	25.0f	},  //�A�C�h���o�g�� 25.0f
	{ "Character\\Slime\\animation\\SlimeAttack.x",	true,	26.0f	},  //�U�� 26.0f
	{ "Character\\Slime\\animation\\SlimeAttack2.x",	true,	70.0f	},  //�U��2 26.0f
	{ "Character\\Slime\\animation\\SlimeGetHit.x",	true,	65.0f	},  //�q�b�g 26.0f
	{ "Character\\Slime\\animation\\SlimeDie.x",	true,	81.0f	},  //���� 41.0f
	//{ "Character\\Slime\\animation\\SlimeDizzy.x",	true,	100.0f	},  //�߂܂� 41.0f
	//{ "Character\\Slime\\animation\\SlimeRun.x",	true,	21.0f	},  //����
	//{ "Character\\Slime\\animation\\SlimeTaunt.x",	true,	21.0f	},  //����
	//{ "Character\\Slime\\animation\\SlimeVictory.x",	true,	81.0f	},  //����
	//{ "Character\\Slime\\animation\\SlimeWalk.x",	true,	31.0f	},  //����
	//{ "Character\\Slime\\animation\\SlimeWalkBack.x",	true,	31.0f	},  //���ɕ���
    //{ "Character\\Slime\\animation\\SlimeWalkLeft.x",	true,	31.0f	},  //���Ɉړ�
	//{ "Character\\Slime\\animation\\SlimeWalkRight.x",	true,	31.0f	},  //�E�Ɉړ�
};

bool CEnemy::IsDeath() const
{
	return mCharaStatus.hp <= 0;
}

// �R���X�g���N�^
CEnemy::CEnemy()
	: CXCharacter(ETag::eEnemy, ETaskPriority::eEnemy)
	, mState(EState::eIdle)
	, mpRideObject(nullptr)
	,mAttackTime(0)
{
	//�C���X�^���X�̐ݒ�
	spInstance = this;

	// ���f���f�[�^�ǂݍ���
	CModelX* model = new CModelX();
	model->Load(MODEL_SLIME);

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
		this, ELayer::eField,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, PLAYER_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });

	//�_���[�W���󂯂�R���C�_�[���쐬
	mpDamageCol = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.5f
	);
	//�_���[�W���󂯂�R���C�_�[��
	//�Փ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageCol->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol->SetCollisionTags({ ETag::eWeapon });
	//�_���[�W���󂯂�R���C�_�[��������ւ��炷
	mpDamageCol->Position(0.0f, 0.3f,0.0f);
}

CEnemy::~CEnemy()
{
	if (mpColliderLine != nullptr)
	{
		delete mpColliderLine;
		mpColliderLine = nullptr;
	}

	//�_���[�W���󂯂�R���C�_�[���폜
	if (mpDamageCol != nullptr)
	{
		delete mpDamageCol;
		mpDamageCol = nullptr;
	}

	if (mpModel != nullptr)
	{
		delete mpModel;
		mpModel = nullptr;
	}
}

CEnemy* CEnemy::Instance()
{
	return spInstance;
}

// �A�j���[�V�����؂�ւ�
void CEnemy::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// �ҋ@���
void CEnemy::UpdateIdle()
{
	if (IsAnimationFinished())
	{
		mState = EState::eIdle2;
	}
}

// �ҋ@2���
void CEnemy::UpdateIdle2()
{
	ChangeAnimation(EAnimType::eIdle2);
	if (IsAnimationFinished())
	{
		mState = EState::eIdleWait;
	}
}

// �U���������̑ҋ@���
void CEnemy::UpdateIdle3()
{
	ChangeAnimation(EAnimType::eIdle4);
	if (IsAnimationFinished())
	{
		mState = EState::eIdle3;
	}
}

// �ҋ@2�̏I���҂�
void CEnemy::UpdateIdleWait()
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
void CEnemy::UpdateAttack()
{
	// �U���A�j���[�V�������J�n
	ChangeAnimation(EAnimType::eAttack);
	// �U���I���҂���Ԃֈڍs
	if (IsAnimationFinished())
	{
		mState = EState::eAttackWait;
	}
}

// �U��2
void CEnemy::UpdateAttack2()
{
	// �U��2�A�j���[�V�������J�n
	ChangeAnimation(EAnimType::eAttack2);
	// �U��2�I���҂���Ԃֈڍs
	if (IsAnimationFinished())
	{
		mState = EState::eAttackWait;
	}
}

// �U���ƍU��2�I���҂�
void CEnemy::UpdateAttackWait()
{
	// �U���ƍU��2�A�j���[�V�������I��������A
	if (IsAnimationFinished())
	{
		// �v���C���[�̍U�����q�b�g�������̑ҋ@��Ԃֈڍs
		mState = EState::eIdle3;
		ChangeAnimation(EAnimType::eIdle4);
	}
}

//�q�b�g
void CEnemy::UpdateHIt()
{
	// �q�b�g�A�j���[�V�������J�n
	ChangeAnimation(EAnimType::eHit);
	if (IsAnimationFinished())
	{
		// �v���C���[�̍U�����q�b�g�������̑ҋ@��Ԃֈڍs
		mState = EState::eIdle3;
		ChangeAnimation(EAnimType::eIdle4);
	}
}

// ���ʎ�
void CEnemy::UpdateDie()
{
	// ���ʎ��̃A�j���[�V�������J�n
	ChangeAnimation(EAnimType::eDie);
	if (IsAnimationFinished())
	{
		Kill();
	}
}

// �߂܂�(����)
void CEnemy::UpdateDizzy()
{
	// �߂܂�(����)�A�j���[�V�������J�n
	ChangeAnimation(EAnimType::eDizzy);
	//if (IsAnimationFinished())
	//{
	//	// �v���C���[�̍U�����q�b�g�������̑ҋ@��Ԃֈڍs
	//	mState = EState::eIdle3;
	//	ChangeAnimation(EAnimType::eIdle4);
	//}
}

//�X�V����
void CEnemy::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;

	mAttackTime++;

	if (mAttackTime > 200)
	{
		mState = EState::eAttack;
	}
	if(mState == EState::eAttack)
	{
		mAttackTime = 0;
	}

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

	// �L�����N�^�[�̍X�V
	CXCharacter::Update();

	mIsGrounded = false;

	//�G�l�~�[�̃f�o�b�N�\��
	static bool debug = false;
	if (CInput::PushKey('Q'))
	{
		debug = !debug;
	}
	if (debug)
	{
		CDebugPrint::Print(" HP %d/%d\n", mCharaStatus.hp, mCharaMaxStatus.hp);
		CDebugPrint::Print(" �U�����Ԍv�� :%d", mAttackTime);
	}
	//mpHpGauge->SetValue(mCharaStatus2.hp);
}

// �Փˏ���
void CEnemy::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (self == mpColliderLine)
	{
		if (other->Layer() == ELayer::eField)
		{
			Position(Position() + hit.adjust);
			mIsGrounded = true;

			if (other->Tag() == ETag::eRideableObject)
			{
				mpRideObject = other->Owner();
			}
		}
	}
}

// �`��
void CEnemy::Render()
{
	CXCharacter::Render();
}

//1���x���A�b�v
void CEnemy::LevelUp()
{
	int level = mCharaStatus.level;
	ChangeLevel(level + 1);
}

//���x����ύX
void CEnemy::ChangeLevel(int level)
{
	//�X�e�[�^�X�̃e�[�u���̃C���f�b�N�X�l�ɕϊ�
	int index = Math::Clamp(level - 1, 0, ENEMY__LEVEL_MAX);
	//�ő�X�e�[�^�X�ɐݒ�
	mCharaMaxStatus = ENEMY_STATUS[index];
	//���݂̃X�e�[�^�X���ő�l�ɂ��邱�ƂŁAHP��
	mCharaStatus = mCharaMaxStatus;

	//mpHpGauge->SetMaxValue(mCharaMaxStatus2.hp);
	//mpHpGauge->SetValue(mCharaStatus2.hp);
}

//��_���[�W����
void CEnemy::TakeDamage(int damage)
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
	//HP��0�ɂȂ�����A
	if (mCharaStatus.hp == 0)
	{
		//���S����
		mState = EState::eDie;
	}
}