#include "CShield.h"
#include "CCollisionManager.h"

// コンストラク
CShield::CShield()
{
	mpShield = CResourceManager::Get<CModel>("Shield");
}

// デストラクタ
CShield::~CShield()
{
}

// 更新
void CShield::Update()
{
}

// 描画
void CShield::Render()
{
	if (mpAttachMtx == nullptr)
	{
		mpShield->Render(Matrix());
	}
	else
	{
		CMatrix sm;
		sm.Scale(100.0f, 100.0f, 100.0f);
		mpShield->Render(sm * (*mpAttachMtx));
	}
}