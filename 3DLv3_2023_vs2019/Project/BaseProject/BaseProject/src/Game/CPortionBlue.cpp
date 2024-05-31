#include "CPortionBlue.h"
#include "CCollisionManager.h"
#include "CColliderSphere.h"
#include "CPlayer.h"

// コンストラク
CPortionBlue::CPortionBlue()
	: CObjectBase(ETag::ePortionBlue, ETaskPriority::eItem)
	, mDefenseUp(false)
{
	mpPortionBlue = CResourceManager::Get<CModel>("Portion3");

	// 球を生成
	mpColliderSphere = new CColliderSphere
	(
		this, ELayer::ePortion,
		0.035f, false
	);
	mpColliderSphere->SetCollisionLayers({ ELayer::ePlayer });
	mpColliderSphere->SetCollisionTags({ ETag::ePlayer });
}

// デストラクタ
CPortionBlue::~CPortionBlue()
{
	SAFE_DELETE(mpColliderSphere)
}

// 衝突処理
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
	 }
}

//　更新
void CPortionBlue::Update()
{
	Rotate(0.0f, 2.0f, 0.0f);
	CPlayer* player = CPlayer::Instance();
	mDefenseUp = player->mDefenseUp;
	CDebugPrint::Print(" 防御: %d\n", mDefenseUp);
}

// 描画
void CPortionBlue::Render()
{
	mpPortionBlue->Render(Matrix());
}