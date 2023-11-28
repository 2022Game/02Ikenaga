#include "CShield.h"
#include "CCollisionManager.h"

CShield::CShield()
{
	mpShield = new CModel();
	mpShield->Load("Item\\Equipment\\Shield\\Shield.obj", "Item\\Equipment\\Shield\\Shield.mtl");
}

CShield::~CShield()
{
	SAFE_DELETE(mpShield);
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