#include "CRich.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "CCane.h"

// ���b�`�̃C���X�^���X
CRich* CRich::spInstance = nullptr;

#define ENEMY_HEIGHT 2.0f

// ���b�`�̃A�j���[�V�����f�[�^�̃e�[�u��
const CRich::AnimData CRich::ANIM_DATA[] =
{
	{ "",										true,	0.0f	},// T�|�[�Y
	{ "Character\\animation\\RichAttack.x",	true,	90.0f	},	// �U�� 41.0f
	//{ "Character\\animation\\RichAttack2.x",	true,	71.0f	},	// �U�� 71.0f
	//{ "Character\\animation\\RichDie.x",	true,	100.0f	},	// ���� 29.0f
	//{ "Character\\animation\\RichGetHit.x",true,	80.0f	},	// �q�b�g  41.0f
	//{ "Character\\animation\\RichIdle.x",	true,	80.0f	},	// �ҋ@ 41.0f
	//{ "Character\\animation\\RichRun.x",	true,	60.0f	},	// ���� 21.0f
	//{ "Character\\animation\\RichVictory.x",	true,	81.0f	},	// ���� 81.0f
	//{ "Character\\animation\\RichWalk.x",	true,	80.0f	},	// ���� 41.0f
};

// �R���X�g���N�^
CRich::CRich()
	: CXCharacter(ETag::eEnemy, ETaskPriority::eDefault)
	, mpRideObject(nullptr)
{
	//�C���X�^���X�̐ݒ�
	spInstance = this;

	// ���f���f�[�^�ǂݍ���
	CModelX* model = CResourceManager::Get<CModelX>("Player2");

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

	mpCane = new CCane();
	mpCane->SetAttachMtx(GetFrameMtx("Hips_Hand_L"));
	mpCane->SetOwner(this);
}

CRich::~CRich()
{
	SAFE_DELETE(mpColliderLine);
}

CRich* CRich::Instance()
{
	return spInstance;
}

// �A�j���[�V�����؂�ւ�
void CRich::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

void CRich::UpdateIdle()
{
	// �ҋ@�A�j���[�V�����ɐ؂�ւ�
	ChangeAnimation(EAnimType::eIdle);
}

//�X�V����
void CRich::Update()
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
void CRich::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
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
void CRich::Render()
{
	CXCharacter::Render();
}