#include "CShield.h"
#include "CCollisionManager.h"

CShield::CShield()
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