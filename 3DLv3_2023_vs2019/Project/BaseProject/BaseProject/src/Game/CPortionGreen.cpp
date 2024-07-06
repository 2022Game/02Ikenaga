#include "CPortionGreen.h"
#include "CCollisionManager.h"
#include "CColliderSphere.h"
#include "CPlayer.h"

#define HEIGHT 0.04f  // 高さ

// コンストラク
CPortionGreen::CPortionGreen()
	: CObjectBase(ETag::ePortionGreen, ETaskPriority::eItem)
{
	mpPortionGreen = CResourceManager::Get<CModel>("Portion2");

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
		0.035f, false
	);
	mpColliderSphere->SetCollisionLayers({ ELayer::ePlayer });
	mpColliderSphere->SetCollisionTags({ ETag::ePlayer });
}

// デストラクタ
CPortionGreen::~CPortionGreen()
{
	// 線分コライダー
	SAFE_DELETE(mpColliderLine)

	// 球コライダー
	SAFE_DELETE(mpColliderSphere)
}

// 衝突処理
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

// 更新
void CPortionGreen::Update()
{
	Rotate(0.0f, 2.0f, 0.0f);
}

// 描画
void CPortionGreen::Render()
{
	mpPortionGreen->Render(Matrix());
}