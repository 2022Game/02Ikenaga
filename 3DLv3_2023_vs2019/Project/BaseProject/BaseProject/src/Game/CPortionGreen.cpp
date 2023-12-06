#include "CPortionGreen.h"
#include "CCollisionManager.h"

CPortionGreen::CPortionGreen()
	: CObjectBase(ETag::eItem2, ETaskPriority::eItem)
{
	mpPortionGreen = CResourceManager::Get<CModel>("Portion2");
}

CPortionGreen::~CPortionGreen()
{
}

void CPortionGreen::Update()
{
}

void CPortionGreen::Render()
{
	mpPortionGreen->Render(Matrix());
}