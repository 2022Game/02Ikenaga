#include "CPortionBlue.h"
#include "CCollisionManager.h"

CPortionBlue::CPortionBlue()
	: CObjectBase(ETag::eItem3, ETaskPriority::eItem)
{
	mpPortionBlue = CResourceManager::Get<CModel>("Portion3");
}

CPortionBlue::~CPortionBlue()
{
}

void CPortionBlue::Update()
{
}

void CPortionBlue::Render()
{
	mpPortionBlue->Render(Matrix());
}