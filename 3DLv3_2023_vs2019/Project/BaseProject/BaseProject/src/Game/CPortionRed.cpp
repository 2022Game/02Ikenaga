#include "CPortionRed.h"
#include "CCollisionManager.h"

CPortionRed::CPortionRed()
	: CObjectBase(ETag::eItem, ETaskPriority::eBackground)
{
	mpPortionRed = new CModel();
	mpPortionRed->Load("Item\\Portion\\PortionRed.obj", "Item\\Portion\\PortionRed.mtl");

	CreateFieldObjects();
}

CPortionRed::~CPortionRed()
{
	if (mpPortionRed != nullptr)
	{
		delete mpPortionRed;
		mpPortionRed = nullptr;
	}
}

void CPortionRed::CreateFieldObjects()
{
}

void CPortionRed::Update()
{
}

void CPortionRed::Render()
{
	mpPortionRed->Render(Matrix());
}