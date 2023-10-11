#include "CBall.h"
#include "CCollisionManager.h"

CBall::CBall()
	: CObjectBase(ETag::eBall, ETaskPriority::eBackground)
{
	mpBall = new CModel();
	mpBall->Load("Field\\Object\\Ball.obj", "Field\\Object\\Ball.mtl");

	CreateFieldObjects();
}

CBall::~CBall()
{
	if (mpBall != nullptr)
	{
		delete mpBall;
		mpBall = nullptr;
	}
}

void CBall::CreateFieldObjects()
{
}

void CBall::Update()
{
}

void CBall::Render()
{
	mpBall->Render(Matrix());
}