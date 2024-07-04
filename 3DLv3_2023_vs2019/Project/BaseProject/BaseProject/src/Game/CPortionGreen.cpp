#include "CPortionGreen.h"
#include "CCollisionManager.h"
#include "CColliderSphere.h"
#include "CPlayer.h"

// �R���X�g���N
CPortionGreen::CPortionGreen()
	: CObjectBase(ETag::ePortionGreen, ETaskPriority::eItem)
{
	mpPortionGreen = CResourceManager::Get<CModel>("Portion2");

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
	SAFE_DELETE(mpColliderSphere)
}

// �Փˏ���
void CPortionGreen::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (self == mpColliderSphere)
	{
		if (other->Layer() == ELayer::ePlayer)
		{
			CPlayer* player = CPlayer::Instance();
			if (player->mHp < player->mMaxHp)
			{
				Kill();
			}
		}
		if (other->Layer() == ELayer::ePortion)
		{
			CVector pushBack = hit.adjust * hit.weight;
			pushBack.Y(0.0f);
			Position(Position() + pushBack);
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