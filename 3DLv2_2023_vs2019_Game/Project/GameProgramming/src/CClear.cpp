#include "CClear.h"
#include"CStageManager.h"
#include "CTask.h"
#include "CCollisionManager.h"

#define STCOUNT 1

int CClear::mSTcount = 1;

int CClear::STcount()
{
	return mSTcount;
}

CClear* CClear::Instance()
{
	return spInstance;
}

CClear* CClear::spInstance = nullptr;

CClear::CClear()
{
	spInstance = this;
}

void CClear::Collision()
{
	mColliderMesh1.ChangePriority();
	mCollider.ChangePriority();
	CCollisionManager::Instance()->Collision(&mCollider, COLLISIONRANGE);
}

void CClear::Collision(CCollider* m, CCollider* o) {
	//相手のコライダタイプの判定
	switch (o->Type())
	{
	case CCollider::ESPHERE: //球コライダの時
		if (CCollider::CCollision(o, m))
		{
			CStageManager::Instance()->LoadStage(0);
			mSTcount = mSTcount += 1;
			if (mSTcount == 100)
			{
				//CStageManager::Instance()->LoadStage(1);
				mSTcount = mSTcount + 101;
			}
			else if (mSTcount == 200)
			{
				//CStageManager::Instance()->LoadStage(2);
				mSTcount = mSTcount + 201;
			}
			else if (mSTcount == 300)
			{
				//CStageManager::Instance()->LoadStage(3);
			}
		}
		break;
	case CCollider::ELINE: //三角コライダの時
		if (CCollider::CCollision(m, o)) 
		{
			//CStageManager::Instance()->LoadStage(0);
		}
		break;
	}
}

CClear::CClear(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale)
	:mCollider(this, &mMatrix, CVector(0.0f, 0.5f, 0.0f), 1.6f)
{
	mSTcount = STCOUNT;
	spInstance = this;
	//モデル,位置,回転,拡縮を設定する
	mpModel = model;  //モデルの設定
	mPosition = position;  //位置の設定
	mRotation = rotation;  //回転の設定
	mScale = scale;  //拡縮の設定
	mColliderMesh1.Set(this, &mMatrix, mpModel);
}

//更新処理
void CClear::Update()
{
	// 行列を更新
	CTransform::Update();
}