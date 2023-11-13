#include "CPortionBlue.h"
#include "CCollisionManager.h"

CPortionBlue::CPortionBlue()
	: CObjectBase(ETag::eItem3, ETaskPriority::eBackground)
{
	mpPortionBlue = new CModel();
	mpPortionBlue->Load("Item\\Portion\\PortionBlue.obj", "Item\\Portion\\PortionBlue.mtl");
}

CPortionBlue::~CPortionBlue()
{
	if (mpPortionBlue != nullptr)
	{
		delete mpPortionBlue;
		mpPortionBlue = nullptr;
	}
}

void CPortionBlue::Update()
{
}

void CPortionBlue::Render()
{
	mpPortionBlue->Render(Matrix());
}