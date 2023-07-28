#include "CNaihu.h"
#include "CCollisionManager.h"
#include"CPlayer.h"

CModel CNaihu::sModel;

void CNaihu::Set(float w, float d)
{
	//スケール設定
	mScale = CVector(0.2f, 0.2f, 0.2f);
}

CNaihu::CNaihu()
	:CCharacter3(1)
	,mLife(10)
	//, mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 0.4f)
{
	//モデルが無いときは読み込む
	if (sModel.Triangles().size() == 0)
	{
		//sModel.Load(OBJ1, MTL1);
	}
	//モデルのポインタ
	mpModel = &sModel;
}

//コンストラクタ
//CWater(位置,回転,拡縮)
CNaihu::CNaihu(const CVector& position, const CVector& rotation, const CVector& scale)
	:CNaihu()  //デフォルトコンストラクタを実行する
{
	//位置、回転、拡縮を設定する
	mPosition = position;  //位置の設定
	mRotation = rotation;  //回転の設定
	mScale = scale;  //拡縮の設定
	CTransform::Update();  //行列の更新
	//目標地点の設定
	//mPoint = mPosition + CVector(0.0f, 0.0f, 0.9f) * mMatrixRotate;
}

// 更新処理
void CNaihu::Update()
{
	//生存時間の判定
	if (mLife-- > 0) {
		CTransform::Update();
		mPosition = mPosition + CVector(0.0f, 0.0f, 0.5f) * mMatrixRotate;
	}
	else {
		//無効にする
		mEnabled = false;
	}
}

void CNaihu::Collision()
{
	mCollider.ChangePriority();
}

//衝突処理
//Collision(コライダ1,コライダ2)
void CNaihu::Collision(CCollider* m, CCollider* o) {
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
			//衝突しない位置まで戻す
			//mPosition = mPosition + adjust;
			mEnabled = false;
		}
		break;
	}
}

//コンストラクタ
//CNaihu(モデル,位置,回転,拡縮)
CNaihu::CNaihu(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale)
	:mLife(10)
	, mCollider(this, &mMatrix, CVector(0.0f, 1.4f, 1.2f), 0.1)
{
	//モデル,位置,回転,拡縮を設定する
	mpModel = model;  //モデルの設定
	mPosition = position;  //位置の設定
	mRotation = rotation;  //回転の設定
	mScale = scale;  //拡縮の設定
}
