#include "CPortionRed.h"
#include "CCollisionManager.h"
#include "CPlayer.h"

#define HEIGHT 0.05f  // ����

// �R���X�g���N
CPortionRed::CPortionRed()
	: CObjectBase(ETag::ePortionRed , ETaskPriority::eItem)
	, mPowerUp(false)
{
	mpPortionRed = CResourceManager::Get<CModel>("Portion");

	// ���𐶐�
	mpColliderLine = new CColliderLine
	(
		this, ELayer::ePortion,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, -HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });

	// ���𐶐�
	mpColliderSphere = new CColliderSphere
	(
		this, ELayer::ePortion,
		0.035f, false
	);
	mpColliderSphere->SetCollisionLayers({ ELayer::ePlayer});
	mpColliderSphere->SetCollisionTags({ ETag::ePlayer });
	mpColliderSphere->Position(0.0f, -0.01f, 0.0f);
}

// �f�X�g���N�^
CPortionRed::~CPortionRed()
{
	// �����R���C�_�[
	SAFE_DELETE(mpColliderLine)

	// ���R���C�_�[
	SAFE_DELETE(mpColliderSphere)
}

// �Փˏ���
void CPortionRed::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (self == mpColliderLine)
	{
		if (other->Layer() == ELayer::eField)
		{
			Position(Position() + hit.adjust * hit.weight);
		}
	}
	else if (self == mpColliderSphere)
	{
		if (other->Layer() == ELayer::ePlayer)
		{
			if (mPowerUp != true)
			{
				Kill();
			}
		}
	}
}

// �X�V
void CPortionRed::Update()
{
	Rotate(0.0f, 2.0f, 0.0f);
	CPlayer* player = CPlayer::Instance();
	mPowerUp = player->IsPowerUp();
}

// �`��
void CPortionRed::Render()
{
	mpPortionRed->Render(Matrix());
}