#include "CPortionRed.h"
#include "CCollisionManager.h"

CPortionRed::CPortionRed()
	: CObjectBase(ETag::eItem, ETaskPriority::eBackground)
{
	mpPortionRed = CResourceManager::Get<CModel>("Portion");
}

CPortionRed::~CPortionRed()
{
}

void CPortionRed::Update()
{
}

void CPortionRed::Render()
{
	mpPortionRed->Render(Matrix());
}