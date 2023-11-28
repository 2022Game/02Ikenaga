#include "CPortionGreen.h"
#include "CCollisionManager.h"

CPortionGreen::CPortionGreen()
	: CObjectBase(ETag::eItem2, ETaskPriority::eBackground)
{
	mpPortionGreen = new CModel();
	mpPortionGreen->Load("Item\\Portion\\PortionGreen.obj", "Item\\Portion\\PortionGreen.mtl");
}

CPortionGreen::~CPortionGreen()
{
	SAFE_DELETE(mpPortionGreen);
}

void CPortionGreen::Update()
{
}

void CPortionGreen::Render()
{
	mpPortionGreen->Render(Matrix());
}