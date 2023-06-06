#include "CHATA.h"
#include"CEffect.h"
#include "CCollisionManager.h"


void CHATA::Collision()
{
	////コライダの優先度変更
	//mCollider1.ChangePriority();
	//mCollider2.ChangePriority();
	//mCollider3.ChangePriority();
	////衝突処理を実行
	//CCollisionManager::Instance()->Collision(&mCollider1, COLLISIONRANGE);
	//CCollisionManager::Instance()->Collision(&mCollider2, COLLISIONRANGE);
	//CCollisionManager::Instance()->Collision(&mCollider3, COLLISIONRANGE);
}

void CHATA::Collision(CCollider* m, CCollider* o) {
	//相手のコライダタイプの判定
	switch (o->Type())
	{
	case CCollider::ESPHERE: //球コライダの時
		//コライダのmとyが衝突しているか判定
		if (CCollider::CCollision(m, o)) {
			//エフェクト生成
			new CEffect(o->Parent()->Position(), 1.0f, 1.0f, "exp.tga", 4, 4, 2);
			//衝突している時は無効にする
			//mEnabled=false;
		}
		break;
	case CCollider::ETRIANGLE: //三角コライダの時
		CVector adjust; //調整値
		//三角コライダと球コライダの衝突判定
		if (CCollider::CollisionTriangleSphere(o, m, &adjust))
		{
			new CEffect(o->Parent()->Position(), 1.0f, 1.0f, "exp.tga", 4, 4, 2);
			//衝突しない位置まで戻す
			mPosition = mPosition + adjust;
		}
		break;
	}

	//コライダもmとoが衝突しているか判定
	//if (CCollider::CCollision(m, o)) {
	//	//エフェクト生成
	//	new CEffect(o->Parent()->Position(), 1.0f, 1.0f, "exp.tga", 4, 4, 2);
	//	//mEnabled = false;
	//}
}

CHATA::CHATA(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale)
/*: mCollider1(this, &mMatrix, CVector(0.0f, 5.0f, 0.0f), 0.8f)
, mCollider2(this, &mMatrix, CVector(0.0f, 5.0f, 20.0f), 0.8f)
, mCollider3(this, &mMatrix, CVector(0.0f, 5.0f, -20.0f), 0.8f)*/
{
	//モデル,位置,回転,拡縮を設定する
	mpModel = model;  //モデルの設定
	mPosition = position;  //位置の設定
	mRotation = rotation;  //回転の設定
	mScale = scale;  //拡縮の設定
	//mColliderMesh1.Set(this, &mMatrix, mpModel);
	//CTransform::Update();
}

//更新処理
void CHATA::Update()
{
	// 行列を更新
	CTransform::Update();
}