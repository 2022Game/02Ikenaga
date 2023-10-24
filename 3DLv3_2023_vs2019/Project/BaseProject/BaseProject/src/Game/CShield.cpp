#include "CShield.h"
#include "CCollisionManager.h"

CShield::CShield()
	: CObjectBase(ETag::eItem, ETaskPriority::eBackground)
{
	mpShield = new CModel();
	mpShield->Load("Item\\Equipment\\Shield\\Shield.obj", "Item\\Equipment\\Shield\\Shield.mtl");

	CreateFieldObjects();
}

CShield::~CShield()
{
	if (mpShield != nullptr)
	{
		delete mpShield;
		mpShield = nullptr;
	}
}

void CShield::CreateFieldObjects()
{
}

void CShield::Update()
{
}

void CShield::Render()
{
	mpShield->Render(Matrix());
}