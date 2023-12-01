#include "CCane.h"
#include "CCollisionManager.h"

CCane::CCane()
{
	mpCane = new CModel();
	mpCane->Load("Character\\Cane.obj", "Character\\Cane.mtl");
}

CCane::~CCane()
{
	SAFE_DELETE(mpCane);
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