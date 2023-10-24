#include "CSword.h"
#include "CCollisionManager.h"

CSword::CSword()
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
	if (mpAttachMtx == nullptr)
	{
		mpSword->Render(Matrix());
	}
	else
	{
		CMatrix sm;
		sm.Scale(100.0f, 100.0f, 100.0f);
		mpSword->Render(sm * (*mpAttachMtx));
	}
}