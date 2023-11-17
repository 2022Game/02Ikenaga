#include "CExp.h"
#include "CCollisionManager.h"

CExp::CExp()
	: CObjectBase(ETag::eExp, ETaskPriority::eExp)
{
	mpExp = new CModel();
	mpExp->Load("Field\\Object\\Ball.obj", "Field\\Object\\Ball.mtl");

	mpColliderSphere = new CColliderSphere
	(
		this,ELayer::eExp,
		1.1f
	);
	mpColliderSphere->SetCollisionLayers({ ELayer::ePlayer });
	mpColliderSphere->Position(-0.03f, 0.1f, 0.05f);
}

CExp::~CExp()
{
	SAFE_DELETE(mpExp);
	SAFE_DELETE(mpColliderSphere);
}

void CExp::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (self == mpColliderSphere)
	{
		if (other->Layer() == ELayer::ePlayer)
		{
			Kill();
		}
	}
}

void CExp::Update()
{
}

void CExp::Render()
{
	mpExp->Render(Matrix());
}