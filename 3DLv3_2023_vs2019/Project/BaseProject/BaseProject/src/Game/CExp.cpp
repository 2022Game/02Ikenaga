#include "CExp.h"
#include "CCollisionManager.h"

CExp::CExp()
	: CObjectBase(ETag::eBall, ETaskPriority::eBackground)
{
	mpExp = new CModel();
	mpExp->Load("Field\\Object\\Ball.obj", "Field\\Object\\Ball.mtl");
}

CExp::~CExp()
{
	if (mpExp != nullptr)
	{
		delete mpExp;
		mpExp = nullptr;
	}
}

void CExp::Update()
{
}

void CExp::Render()
{
	mpExp->Render(Matrix());
}