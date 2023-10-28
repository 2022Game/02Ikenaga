#include "CEnemy.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "CInput.h"

// �G�l�~�[�̃C���X�^���X
CEnemy* CEnemy::spInstance = nullptr;

#define MODEL_SLIME "Character\\Slime\\Slime.x"

#define PLAYER_HEIGHT 2.0f

// �G�l�~�[�̃A�j���[�V�����f�[�^�̃e�[�u��
const CEnemy::AnimData CEnemy::ANIM_DATA[] =
{
	{ "",										true,	0.0f	},	// T�|�[�Y
	//{ "Character\\Slime\\animation\\SlimeAttack.x",	true,	26.0f	},  //�U��
	{ "Character\\Slime\\animation\\SlimeAttack2.x",	true,	26.0f	},  //�U��2
	//{ "Character\\Slime\\animation\\SlimeDie.x",	true,	81.0f	},  //���� 41.0f
	//{ "Character\\Slime\\animation\\SlimeDizzy.x",	true,	100.0f	},  //�߂܂� 41.0f
	//{ "Character\\Slime\\animation\\SlimeGetHit.x",	true,	100.0f	},  //�q�b�g 26.0f
	//{ "Character\\Slime\\animation\\SlimeIdleBattle.x",	true,	50.0f	},  //�A�C�h���o�g�� 25.0f
	//{ "Character\\Slime\\animation\\SlimeIdleNormal.x",	true,	51.0f	},  //�A�C�h���ʏ�
	//{ "Character\\Slime\\animation\\SlimeRun.x",	true,	21.0f	},  //����
	//{ "Character\\Slime\\animation\\SlimeSenseSomethingRoutine.x",	true,	71.0f	},  //����
	//{ "Character\\Slime\\animation\\SlimeSenseSomethingStart.x",	true,	63.0f	},  //�J�n�̌���
	//{ "Character\\Slime\\animation\\SlimeTaunt.x",	true,	21.0f	},  //����
	//{ "Character\\Slime\\animation\\SlimeVictory.x",	true,	81.0f	},  //����
	//{ "Character\\Slime\\animation\\SlimeWalk.x",	true,	31.0f	},  //����
	//{ "Character\\Slime\\animation\\SlimeWalkBack.x",	true,	31.0f	},  //���ɕ���
    //{ "Character\\Slime\\animation\\SlimeWalkLeft.x",	true,	31.0f	},  //���Ɉړ�
	//{ "Character\\Slime\\animation\\SlimeWalkRight.x",	true,	31.0f	},  //�E�Ɉړ�
};

// �R���X�g���N�^
CEnemy::CEnemy()
	: CXCharacter(ETag::eEnemy, ETaskPriority::eDefault)
	, mpRideObject(nullptr)
{
	//�C���X�^���X�̐ݒ�
	spInstance = this;

	// ���f���f�[�^�ǂݍ���
	CModelX* model = new CModelX();
	model->Load(MODEL_SLIME);

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
}

CEnemy::~CEnemy()
{
	if (mpColliderLine != nullptr)
	{
		delete mpColliderLine;
		mpColliderLine = nullptr;
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

void CEnemy::UpdateIdle()
{
	// �ҋ@�A�j���[�V�����ɐ؂�ւ�
	ChangeAnimation(EAnimType::eIdle);
}

//�X�V����
void CEnemy::Update()
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
