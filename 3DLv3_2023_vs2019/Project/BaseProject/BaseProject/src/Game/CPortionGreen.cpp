#include "CPortionGreen.h"
#include "CCollisionManager.h"
#include "CColliderSphere.h"
#include "CPlayer.h"

#define HEIGHT 0.04f  // ����

// �R���X�g���N
CPortionGreen::CPortionGreen()
	: CObjectBase(ETag::ePortionGreen, ETaskPriority::eItem)
{
	mpPortionGreen = CResourceManager::Get<CModel>("Portion2");

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
	mpColliderSphere->SetCollisionLayers({ ELayer::ePlayer });
	mpColliderSphere->SetCollisionTags({ ETag::ePlayer });
}

// �f�X�g���N�^
CPortionGreen::~CPortionGreen()
{
	// �����R���C�_�[
	SAFE_DELETE(mpColliderLine)

	// ���R���C�_�[
	SAFE_DELETE(mpColliderSphere)
}

// �Փˏ���
void CPortionGreen::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
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
			CPlayer* player = CPlayer::Instance();
			if (player->mHp < player->mMaxHp)
			{
				Kill();
			}
		}
	}
}

// �X�V
void CPortionGreen::Update()
{
	Rotate(0.0f, 2.0f, 0.0f);
}

// �`��
void CPortionGreen::Render()
{
	mpPortionGreen->Render(Matrix());
}