#include "CPortionBlue.h"
#include "CCollisionManager.h"
#include "CColliderSphere.h"
#include "CPlayer.h"

// �R���X�g���N
CPortionBlue::CPortionBlue()
	: CObjectBase(ETag::ePortionBlue, ETaskPriority::eItem)
	, mDefenseUp(false)
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
			 if (mDefenseUp != true)
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

//�@�X�V
void CPortionBlue::Update()
{
	Rotate(0.0f, 2.0f, 0.0f);
	CPlayer* player = CPlayer::Instance();
	mDefenseUp = player->IsDefenseUp();
}

// �`��
void CPortionBlue::Render()
{
	mpPortionBlue->Render(Matrix());
}