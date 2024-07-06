#include "CPortionBlue.h"
#include "CCollisionManager.h"
#include "CColliderSphere.h"
#include "CPlayer.h"

#define HEIGHT 0.04f  // 高さ

// コンストラク
CPortionBlue::CPortionBlue()
	: CObjectBase(ETag::ePortionBlue, ETaskPriority::eItem)
	, mDefenseUp(false)
{
	mpPortionBlue = CResourceManager::Get<CModel>("Portion3");

	// 線を生成
	mpColliderLine = new CColliderLine
	(
		this, ELayer::ePortion,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, -HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });

	// 球を生成
	mpColliderSphere = new CColliderSphere
	(
		this, ELayer::ePortion,
		0.035f, false,0.1f
	);
	mpColliderSphere->SetCollisionLayers({ ELayer::ePlayer });
	mpColliderSphere->SetCollisionTags({ ETag::ePlayer });
}

// デストラクタ
CPortionBlue::~CPortionBlue()
{
	// 線分コライダー
	SAFE_DELETE(mpColliderLine)

	// 球コライダー
	SAFE_DELETE(mpColliderSphere)
}

// 衝突処理
 void CPortionBlue::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
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
	mDefenseUp = player->IsDefenseUp();
}

// 描画
void CPortionBlue::Render()
{
	mpPortionBlue->Render(Matrix());
}