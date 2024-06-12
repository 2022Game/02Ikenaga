#include "CSky.h"

// コンストラク
CSky::CSky()
	: CObjectBase(ETag::eSky, ETaskPriority::eBackground)
{
	mpModel = CResourceManager::Get<CModel>("Sky");
}

// デストラクタ
CSky::~CSky()
{
}

// 更新
void CSky::Update()
{
	Rotate(0.0f, 0.01f, 0.0f);
}

// 描画
void CSky::Render()
{
	mpModel->Render(Matrix());
}