#include "CClear.h"
#include "CCollisionManager.h"
#include"CPlayer.h"
#include"CStageManager.h"

#define HP 1

int CClear::sHp = 0;

int CClear::Hp()
{
	return sHp;
}

CClear* CClear::Instance()
{
	return spInstance;
}

CClear* CClear::spInstance = nullptr;

CClear::CClear()
{

}

void CClear::Collision()
{
	mColliderMesh1.ChangePriority();
	mCollider.ChangePriority();
	CCollisionManager::Instance()->Collision(&mCollider, COLLISIONRANGE);
}

CClear::CClear(const CVector& position, const CVector& rotation, const CVector& scale)
{
	//位置、回転、拡縮を設定する
	mPosition = position;  //位置の設定
	mRotation = rotation;  //回転の設定
	mScale = scale;  //拡縮の設定
	//CTransform::Update();  //行列の更新
}

void CClear::Collision(CCollider* m, CCollider* o) {
	//相手のコライダタイプの判定
	switch (o->Type())
	{
	case CCollider::ESPHERE: //球コライダの時
		//コライダのmとoが衝突しているか判定
		if (CCollider::CCollision(m, o))
		{
			sHp = 0;
			CStageManager::Instance()->LoadStage(1);
		}
		break;
	case CCollider::ELINE: //三角コライダの時
		if (CCollider::CCollision(m, o)) 
		{
			sHp = 0;
		}
		break;
	}
}

CClear::CClear(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale)
	:mCollider(this, &mMatrix, CVector(0.0f, 0.5f, 0.0f), 1.6f)
{
	sHp = HP;
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