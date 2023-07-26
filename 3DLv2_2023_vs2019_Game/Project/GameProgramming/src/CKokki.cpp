#include "CKokki.h"
#include"CEffect.h"
#include "CCollisionManager.h"


void CKokki::Collision()
{
	mColliderMesh1.ChangePriority();
}

void CKokki::Collision(CCollider* m, CCollider* o) {
	//相手のコライダタイプの判定
	switch (o->Type())
	{
	case CCollider::ESPHERE: //球コライダの時
		//コライダのmとyが衝突しているか判定
		if (CCollider::CCollision(m, o)) {
			//エフェクト生成
			//new CEffect(o->Parent()->Position(), 1.0f, 1.0f, "exp.tga", 4, 4, 2);
			//衝突している時は無効にする
			//mEnabled=false;
		}
		break;
	case CCollider::ETRIANGLE: //三角コライダの時
		CVector adjust; //調整値
		//三角コライダと球コライダの衝突判定
		if (CCollider::CollisionTriangleSphere(o, m, &adjust))
		{
			//new CEffect(o->Parent()->Position(), 1.0f, 1.0f, "exp.tga", 4, 4, 2);
			//衝突しない位置まで戻す
			//mPosition = mPosition + adjust;
		}
		break;
	}
}

CKokki::CKokki(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale)
{
	//モデル,位置,回転,拡縮を設定する
	mpModel = model;  //モデルの設定
	mPosition = position;  //位置の設定
	mRotation = rotation;  //回転の設定
	mScale = scale;  //拡縮の設定
	mColliderMesh1.Set(this, &mMatrix, mpModel);
}

//更新処理
void CKokki::Update()
{
	// 行列を更新
	CTransform::Update();
}