#include "CEnemy2.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "CInput.h"

// �G�l�~�[2�̃C���X�^���X
CEnemy2* CEnemy2::spInstance = nullptr;

#define MODEL_MUSHROOM "Character\\Mushroom\\Mushroom.x"

#define PLAYER_HEIGHT 2.0f

// �G�l�~�[�̃A�j���[�V�����f�[�^�̃e�[�u��
const CEnemy2::AnimData CEnemy2::ANIM_DATA[] =
{
	{ "",										true,	0.0f	},// T�|�[�Y
	//{ "Character\\Mushroom\\animation\\MushroomAttack.x",	true,	26.0f	},	// �U��
	//{ "Character\\Mushroom\\animation\\MushroomAttack2.x",	true,	70.0f	},	// �U�� 26.0f
	//{ "Character\\Mushroom\\animation\\MushroomAttack3.x",	true,	70.0f	},	// �U�� 26.0f
	//{ "Character\\Mushroom\\animation\\MushroomDie.x",	true,	70.0f	},	// �U�� 26.0f
	//{ "Character\\Mushroom\\animation\\MushroomDizzy.x",	true,	70.0f	},	// �߂܂� 41.0f
	{ "Character\\Mushroom\\animation\\MushroomGetHit.x",	true,	70.0f	},	// �q�b�g 23.0f
};

// �R���X�g���N�^
CEnemy2::CEnemy2()
	: CXCharacter(ETag::eEnemy, ETaskPriority::eDefault)
	, mpRideObject(nullptr)
{
	//�C���X�^���X�̐ݒ�
	spInstance = this;

	// ���f���f�[�^�ǂݍ���
	CModelX* model = new CModelX();
	model->Load(MODEL_MUSHROOM);

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

CEnemy2::~CEnemy2()
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
