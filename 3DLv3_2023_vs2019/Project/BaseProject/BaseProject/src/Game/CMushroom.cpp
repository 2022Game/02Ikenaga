#include "CMushroom.h"
#include "CPlayer.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "CInput.h"

// �}�b�V�����[���̃C���X�^���X
CMushroom* CMushroom::spInstance = nullptr;

#define MODEL_MUSHROOM "Character\\Enemy\\Mushroom\\Mushroom.x"

#define ENEMY_HEIGHT 2.0f
#define WITHIN_RANGE 40.0f    // �͈͓�

// �}�b�V�����[���̃A�j���[�V�����f�[�^�̃e�[�u��
const CMushroom::AnimData CMushroom::ANIM_DATA[] =
{
	{ "",										true,	0.0f	},// T�|�[�Y
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomIdlePlant.x",	true,	21.0f	},	        // �A�� 21.0f
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomIdlePlantToBattle.x",	true,	70.0f	},	// �A�����炫�̂� 21.0f
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomIdleBattle2.x",	true,	41.0f	},	// �ҋ@2 18.0f
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomIdleBattle.x",	true,	41.0f	},	    // �ҋ@ 18.0f
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomAttack.x",	true,	80.0f	},	// �U�� 26.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomAttack2.x",	true,	70.0f	},	// �U�� 26.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomAttack3.x",	true,	70.0f	},	// �U�� 26.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomDie.x",	true,	70.0f	},	// �U�� 26.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomDizzy.x",	true,	70.0f	},	// �߂܂� 41.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomGetHit.x",	true,	70.0f	},	// �q�b�g 23.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomRun.x",	true,	40.0f	},	//���� 17.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomSenseSomethingMaintain.x",	true,	121.0f	},	//���� 121.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomSenseSomethingStart.x",	true,	25.0f	},	//�J�n�̌��� 25.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomTaunting.x",	true,	80.0f	},	//���� 41.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomVictory.x",	true,	61.0f	},	//���� 61.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomWalk.x",	true,	31.0f	},	//���� 31.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomWalk2.x",	true,	31.0f	},	//����2 31.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomWalk3.x",	true,	31.0f	},	//����3 31.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomWalk4.x",	true,	31.0f	},	//����4 31.0f
};

// �R���X�g���N�^
CMushroom::CMushroom()
	: mpRideObject(nullptr)
{
	//�C���X�^���X�̐ݒ�
	spInstance = this;

	// ���f���f�[�^�ǂݍ���
	CModelX* model = CResourceManager::Get<CModelX>("Mushroom");

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
		0.3f, false, 5.0f
	);
	mpColliderSphere->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy2 });
	mpColliderSphere->Position(0.0f, 0.2f, 0.0f);

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

	mpDamageCol->SetEnable(false);

	// �_���[�W��^����R���C�_�[
	mpAttackCol = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		0.6f, false
	);
	mpAttackCol->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackCol->SetCollisionTags({ ETag::ePlayer });
	mpAttackCol->Position(0.0f, -0.1f, 0.0f);

	// �U���R���C�_�[���}�b�V�����[���̓��̍s��ɃA�^�b�`
	const CMatrix* headMty = GetFrameMtx("Armature_mushroom_spine03");
	mpAttackCol->SetAttachMtx(headMty);

	// �ŏ��̍U���R���C�_�[�𖳌��ɂ��Ă���
	mpAttackCol->SetEnable(false);
}

CMushroom::~CMushroom()
{
	SAFE_DELETE(mpColliderLine);
	SAFE_DELETE(mpColliderSphere);
	SAFE_DELETE(mpDamageCol);
	SAFE_DELETE(mpAttackCol);
}

CMushroom* CMushroom::Instance()
{
	return spInstance;
}

// �A�j���[�V�����؂�ւ�
void CMushroom::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// �키�O�̑ҋ@���
void CMushroom::UpdateIdle()
{
	if (IsAnimationFinished())
	{
		mState = EState::eIdle2;
	}
}

// �키�O�̑ҋ@���2
void CMushroom::UpdateIdle2()
{
	ChangeAnimation(EAnimType::eIdle2);
	if (IsAnimationFinished())
	{
		mState = EState::eIdle3;
	}
}

// �ҋ@���3
void CMushroom::UpdateIdle3()
{
	ChangeAnimation(EAnimType::eIdle3);
}

// �U��
void CMushroom::UpdateAttack()
{
	ChangeAnimation(EAnimType::eAttack);
	AttackStart();
	// �U��2�I���҂���Ԃֈڍs
	mState = EState::eAttackWait;
}

// �U���I���҂�
void  CMushroom::UpdateAttackWait()
{
	if (IsAnimationFinished())
	{
		AttackEnd();
		mState = EState::eIdle3;
	}
}

// �X�V����
void CMushroom::Update()
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
		// �U���I���҂�
	case EState::eAttackWait:
		UpdateAttackWait();
	}

	CPlayer* player = CPlayer::Instance();
	float vectorp = (player->Position() - Position()).Length();
	if (vectorp <= WITHIN_RANGE && mState != EState::eIdle3)
	{
		UpdateIdle();
	}

	// �L�����N�^�[�̍X�V
	CXCharacter::Update();

	mpAttackCol->Update();

	mIsGrounded = false;

	if (CInput::PushKey('Q'))
	{
		mState = EState::eAttack;
	}
}

// �Փˏ���
void CMushroom::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
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
void CMushroom::AttackStart()
{
	CXCharacter::AttackStart();
	// �U�����n�܂�����A�U������p�̃R���C�_�[���I���ɂ���
	mpAttackCol->SetEnable(true);
}

// �U���I��
void CMushroom::AttackEnd()
{
	CXCharacter::AttackEnd();
	// �U�����I���΁A�U������p�̃R���C�_�[���I�t�ɂ���
	mpAttackCol->SetEnable(false);
}

// �`��
void CMushroom::Render()
{
	CXCharacter::Render();
}


// ���S����
void CMushroom::Death()
{
	// �}�b�V�����[���̎��S����
	CEnemy::Death();
	// ���S��Ԃֈڍs
	//mState = EState::eDie;
}