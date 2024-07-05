#include "CPortionGreen.h"
#include "CCollisionManager.h"
#include "CColliderSphere.h"
#include "CPlayer.h"

// コンストラク
CPortionGreen::CPortionGreen()
	: CObjectBase(ETag::ePortionGreen, ETaskPriority::eItem)
{
	mpPortionGreen = CResourceManager::Get<CModel>("Portion2");

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
	SAFE_DELETE(mpColliderSphere)
}

// 衝突処理
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