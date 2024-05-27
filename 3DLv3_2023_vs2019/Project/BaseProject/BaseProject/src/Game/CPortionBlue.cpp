#include "CPortionBlue.h"
#include "CCollisionManager.h"
#include "CColliderSphere.h"

// �R���X�g���N
CPortionBlue::CPortionBlue()
	: CObjectBase(ETag::ePortionBlue, ETaskPriority::eItem)
{
	mpPortionBlue = CResourceManager::Get<CModel>("Portion3");

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
CPortionBlue::~CPortionBlue()
{
	SAFE_DELETE(mpColliderSphere)
}

// �Փˏ���
 void CPortionBlue::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	 if (self == mpColliderSphere)
	 {
		 if(other->Layer() == ELayer::ePlayer)
		 {
			 Kill();
		 }
	 }
}

//�@�X�V
void CPortionBlue::Update()
{
	Rotate(0.0f, 2.0f, 0.0f);
}

// �`��
void CPortionBlue::Render()
{
	mpPortionBlue->Render(Matrix());
}