#include "CEnemy.h"
#include "CEffect.h"
#include "CCollisionManager.h"

#define MODEL_SLIME "Character\\Slime\\Slime.x"

#define PLAYER_HEIGHT 2.0f

// �G�l�~�[�̃C���X�^���X
CEnemy* CEnemy::spInstance = nullptr;

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

	// CXCharacter�̏�����
	Init(model);

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

//�X�V����
void CEnemy::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;

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
