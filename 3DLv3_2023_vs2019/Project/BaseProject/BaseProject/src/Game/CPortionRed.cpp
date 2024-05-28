#include "CPortionRed.h"
#include "CCollisionManager.h"

// �R���X�g���N
CPortionRed::CPortionRed()
	: CObjectBase(ETag::ePortionRed , ETaskPriority::eItem)
{
	mpPortionRed = CResourceManager::Get<CModel>("Portion");

	// ���𐶐�
	mpColliderSphere = new CColliderSphere
	(
		this, ELayer::ePortion,
		0.035f, false
	);
	mpColliderSphere->SetCollisionLayers({ ELayer::ePlayer });
	mpColliderSphere->SetCollisionTags({ ETag::ePlayer });
	mpColliderSphere->Position(0.0f, -0.01f, 0.0f);
}

// �f�X�g���N�^
CPortionRed::~CPortionRed()
{
	SAFE_DELETE(mpColliderSphere)
}

// �Փˏ���
void CPortionRed::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (self == mpColliderSphere)
	{
		if (other->Layer() == ELayer::ePlayer)
		{
			Kill();
		}
	}
}

// �X�V
void CPortionRed::Update()
{
	Rotate(0.0f, 2.0f, 0.0f);
}

// �`��
void CPortionRed::Render()
{
	mpPortionRed->Render(Matrix());
}