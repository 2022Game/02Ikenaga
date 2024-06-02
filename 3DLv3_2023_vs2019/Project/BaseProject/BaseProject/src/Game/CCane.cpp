#include "CCane.h"
#include "CCollisionManager.h"

// コンストラク
CCane::CCane()
{
	mpCane = CResourceManager::Get<CModel>("Cane");
}

// デストラクタ
CCane::~CCane()
{
}

// 更新
void CCane::Update()
{

}

// 描画
void CCane::Render()
{
	if (mpAttachMtx == nullptr)
	{
		mpCane->Render(Matrix());
	}
	else
	{
		CMatrix sm;
		sm.Scale(10000.0f, 10000.0f, 10000.0f);
		mpCane->Render(sm * (*mpAttachMtx));
	}
}