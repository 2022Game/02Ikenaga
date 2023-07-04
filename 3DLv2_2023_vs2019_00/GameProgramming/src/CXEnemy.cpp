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
	// //体
	mColSphereBody.Matrix(&mpCombinedMatrix[1]);
	//頭
	mColSphereHead.Matrix(&mpCombinedMatrix[1]);
	//剣
	mColSphereSword0.Matrix(&mpCombinedMatrix[26]);
	//剣
	mColSphereSword1.Matrix(&mpCombinedMatrix[26]);
	//剣
	mColSphereSword2.Matrix(&mpCombinedMatrix[26]);
}

CXEnemy::CXEnemy()
	:mColSphereBody(this, nullptr, CVector(0.5, -1.0f, 0.0f), 1.0f, CCollider::ETag::EBODY)
	, mColSphereHead(this, nullptr, CVector(0.0f, 1.0f, 0.0f), 1.5f)
	, mColSphereSword0(this, nullptr, CVector(0.7f, 3.5f, -0.2f), 0.5f,CCollider::ETag::ESWORD)
	, mColSphereSword1(this, nullptr, CVector(0.5f, 2.5f, -0.2f), 0.5f, CCollider::ETag::ESWORD)
	, mColSphereSword2(this, nullptr, CVector(0.3f, 1.5f, -0.2f), 0.5f, CCollider::ETag::ESWORD)
{
}