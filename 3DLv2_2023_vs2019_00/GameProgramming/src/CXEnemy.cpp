#include "CXEnemy.h"
//#include "CCollider.h"

void CXEnemy::Collision(CCollider* m, CCollider* o)
{
	//相手のコライダタイプの判定
	switch (o->Parent()->Tag())
	{
		//case ETag::EPL/YER && CCollider::ESPHERE:
	case EPLAYER://球コライダの時
		if (o->Tag() == CCollider::ETag::ESWORD)
		{
			//コライダのmとoが衝突しているか判定
			if (CCollider::Collision(o, m))
			{
				ChangeAnimation(11, false, 30);
			}
		}
		else if (o->Tag() == CCollider::ETag::EBODY && o->Tag() == CCollider::ETag::ESWORD)
		{
			if (CCollider::Collision(o, m))
			{
				ChangeAnimation(0, true, 60);
			}
		}
		break;
	}
}

void CXEnemy::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//合成行列の設定
	//頭
	mColSphereHead.Matrix(&mpCombinedMatrix[11]);
	//剣
	mColSphereSword.Matrix(&mpCombinedMatrix[21]);
	//体
	mColSphereBody.Matrix(&mpCombinedMatrix[8]);
}

CXEnemy::CXEnemy()
	:mColSphereBody(this, nullptr, CVector(), 0.5f,CCollider::ETag::EBODY)
	, mColSphereHead(this, nullptr, CVector(0.0f, 5.0f, -3.0f), 0.5f)
	, mColSphereSword(this, nullptr, CVector(-10.0f, 10.0f, 50.0f), 0.3f,CCollider::ETag::ESWORD)
{
}