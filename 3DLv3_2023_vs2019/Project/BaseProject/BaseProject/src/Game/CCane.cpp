#include "CCane.h"
#include "CCollisionManager.h"

CCane::CCane()
{
	mpCane = CResourceManager::Get<CModel>("Cane");
}

CCane::~CCane()
{
}

void CCane::Update()
{

}

void CCane::Render()
{
	if (mpAttachMtx == nullptr)
	{
		mpCane->Render(Matrix());
	}
	else
	{
		CMatrix sm;
		sm.Scale(10000.0f, 10000.0f, 10000.0f);
		mpCane->Render(sm * (*mpAttachMtx));
	}
}