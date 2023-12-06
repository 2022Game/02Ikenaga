#include "CShield.h"
#include "CCollisionManager.h"

CShield::CShield()
{
	mpShield = CResourceManager::Get<CModel>("Shield");
}

CShield::~CShield()
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