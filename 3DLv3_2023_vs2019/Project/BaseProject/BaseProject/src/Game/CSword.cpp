#include "CSword.h"
#include "CCollisionManager.h"

CSword::CSword()
	: CObjectBase(ETag::eItem, ETaskPriority::eBackground)
{
	mpSword = new CModel();
	mpSword->Load("Item\\Equipment\\Sword\\Sword.obj", "Item\\Equipment\\Sword\\Sword.mtl");
	//mpSword->Load("Item\\Equipment\\Shield\\Shield.obj", "Item\\Equipment\\Shield\\Shield.mtl");

	CreateFieldObjects();
}

CSword::~CSword()
{
	if (mpSword != nullptr)
	{
		delete mpSword;
		mpSword = nullptr;
	}
}

void CSword::CreateFieldObjects()
{
}

void CSword::Update()
{
}

void CSword::Render()
{
	mpSword->Render(Matrix());
}