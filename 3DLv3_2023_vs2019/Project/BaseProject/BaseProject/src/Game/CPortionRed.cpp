#include "CPortionRed.h"
#include "CCollisionManager.h"
#include "CPlayer.h"

// コンストラク
CPortionRed::CPortionRed()
	: CObjectBase(ETag::ePortionRed , ETaskPriority::eItem)
	, mPowerUp(false)
{
	mpPortionRed = CResourceManager::Get<CModel>("Portion");

	// 球を生成
	mpColliderSphere = new CColliderSphere
	(
		this, ELayer::ePortion,
		0.035f, false
	);
	mpColliderSphere->SetCollisionLayers({ ELayer::ePlayer });
	mpColliderSphere->SetCollisionTags({ ETag::ePlayer });
	mpColliderSphere->Position(0.0f, -0.01f, 0.0f);
}

// デストラクタ
CPortionRed::~CPortionRed()
{
	SAFE_DELETE(mpColliderSphere)
}

// 衝突処理
void CPortionRed::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (self == mpColliderSphere)
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

// 更新
void CPortionRed::Update()
{
	Rotate(0.0f, 2.0f, 0.0f);
	CPlayer* player = CPlayer::Instance();
	mPowerUp = player->mPowerUp;
}

// 描画
void CPortionRed::Render()
{
	mpPortionRed->Render(Matrix());
}