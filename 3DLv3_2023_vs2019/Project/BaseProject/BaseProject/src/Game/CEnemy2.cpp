#include "CEnemy2.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "CInput.h"

// �G�l�~�[2�̃C���X�^���X
CEnemy2* CEnemy2::spInstance = nullptr;

#define MODEL_MUSHROOM "Character\\Enemy\\Mushroom\\Mushroom.x"
#define MODEL_RICH "Character\\Rich.x"

#define ENEMY_HEIGHT 2.0f

// �G�l�~�[�̃A�j���[�V�����f�[�^�̃e�[�u��
const CEnemy2::AnimData CEnemy2::ANIM_DATA[] =
{
	{ "",										true,	0.0f	},// T�|�[�Y
	{ "Character\\Enemy\\Mushroom\\animation\\MushroomAttack.x",	true,	80.0f	},	// �U�� 26.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomAttack2.x",	true,	70.0f	},	// �U�� 26.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomAttack3.x",	true,	70.0f	},	// �U�� 26.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomDie.x",	true,	70.0f	},	// �U�� 26.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomDizzy.x",	true,	70.0f	},	// �߂܂� 41.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomGetHit.x",	true,	70.0f	},	// �q�b�g 23.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomIdleBattle.x",	true,	41.0f	},	// �ҋ@ 18.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomIdleBattle2.x",	true,	41.0f	},	// �ҋ@2 41.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomIdlePlant.x",	true,	21.0f	},	// �A�� 21.0f
	//{ "Character\\Enemy\\Mushroom\\animation\\MushroomIdlePlantToBattle.x",	true,	70.0f	},	// �A�����炫�̂� 21.0f
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
CEnemy2::CEnemy2()
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
}

CEnemy2::~CEnemy2()
{
	SAFE_DELETE(mpColliderLine);
}

CEnemy2* CEnemy2::Instance()
{
	return spInstance;
}

// �A�j���[�V�����؂�ւ�
void CEnemy2::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

void CEnemy2::UpdateIdle()
{
	// �ҋ@�A�j���[�V�����ɐ؂�ւ�
	ChangeAnimation(EAnimType::eIdle);
}

//�X�V����
void CEnemy2::Update()
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
	}

	// �L�����N�^�[�̍X�V
	CXCharacter::Update();

	mIsGrounded = false;
}

// �Փˏ���
void CEnemy2::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
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
void CEnemy2::Render()
{
	CXCharacter::Render();
}


// ���S����
void CEnemy2::Death()
{
	// �G�l�~�[�̎��S����
	CEnemy::Death();
	// ���S��Ԃֈڍs
	//mState = EState::eDie;
}