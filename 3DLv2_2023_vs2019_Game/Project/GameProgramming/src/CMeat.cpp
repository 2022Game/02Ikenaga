#include "CMeat.h"
#include "CCollisionManager.h"
#include"CPlayer.h"

CModel CMeat::sModel;

//移動速度
#define VELOCITYX CVector(0.0f,0.0f,0.09f)//移動速度
#define VELOCITY CVector(0.0f,0.0f,0.1f)//9
#define VELOCITY2 0.02f
#define ROTATION_Y CVector(0.0f,-0.3f,0.0f)

void CMeat::Set(float w, float d)
{
	//スケール設定
	mScale = CVector(0.5f, 0.5f, 0.5f);
}

CMeat::CMeat()
	:CCharacter3(1)
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
//CMeat(位置,回転,拡縮)
CMeat::CMeat(const CVector& position, const CVector& rotation, const CVector& scale)
	:CMeat()  //デフォルトコンストラクタを実行する
{
	//位置、回転、拡縮を設定する
	mPosition = position;  //位置の設定
	mRotation = rotation;  //回転の設定
	mScale = scale;  //拡縮の設定
	CTransform::Update();  //行列の更新
	//目標地点の設定
	mPoint = mPosition + CVector(0.0f, 0.0f, 10.0f) * mMatrixRotate;
}

// 更新処理
void CMeat::Update()
{
	//目標地点までのベクトルを求める
	CVector vp = mPoint - mPosition;
	//課題
	//左ベクトルとの内積を求める
	float dx = vp.Dot(mMatrixRotate.VectorX());
	//上ベクトルとの内積を求める
	float dy = vp.Dot(mMatrixRotate.VectorY());
	const float margin = 0.1f;
	//左右方向へ回転
	if (dx > margin)
	{
		//mRotation = mRotation + CVector(0.0f, 1.0f, 0.0f);  //左へ回転
	}
	else if (dx < -margin)
	{
		//課題
		//mRotation = mRotation + CVector(0.0f, -1.0f, 0.0f);  //右へ回転
	}
	//上下方向へ回転
	if (dy > margin)
	{
		//mRotation = mRotation + CVector(-1.0f, 0.0f, 0.0f);  //上へ回転
		//mPosition = mPosition + mMatrixRotate.VectorZ() * VELOCITY2;
	}
	else if (dy < -margin)
	{
		//課題
		//mRotation = mRotation + CVector(1.0f, 0.0f, 0.0f);  //下へ回転
		//mPosition = mPosition + mMatrixRotate.VectorZ() * VELOCITY2;
	}
	//機体前方へ移動する
	//mPosition = mPosition + mMatrixRotate.VectorZ() * VELOCITY2;
	CTransform::Update(); //行列更新
	//およそ3秒毎に目標地点を更新
	int r = rand() % 180; //rand()は整数の乱数を返す
						  //% 180は180で割った余りを求める
	mPosition = mPosition + ROTATION_Y * mMatrixRotate;
}

void CMeat::Collision()
{
	mCollider.ChangePriority();
	mLine.ChangePriority();
	CCollisionManager::Instance()->Collision(&mCollider, COLLISIONRANGE);
}

//衝突処理
//Collision(コライダ1,コライダ2)
void CMeat::Collision(CCollider* m, CCollider* o) {
	//相手のコライダタイプの判定
	switch (m->ELINE)
	{
	case CCollider::ELINE: //三角コライダの時
		if (o->Type() == CCollider::ETRIANGLE)
		{
			CVector adjust; //調整値
			//三角コライダと球コライダの衝突判定
			if (CCollider::CollisionTriangleSphere(o, m, &adjust))
			{
				//衝突しない位置まで戻す
				mPosition = mPosition + adjust;
				CTransform::Update();
			}
		}
		break;
	}
}

//コンストラクタ
//CMeat(モデル,位置,回転,拡縮)
CMeat::CMeat(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale)
	:mLine(this, &mMatrix, CVector(0.0f, 1.0f, 0.0f), CVector(0.0f, -1.0f, 0.0f))
	, mCollider(this, &mMatrix, CVector(0.0f, 3.0f, 0.0f), 0.3f)
{
	//モデル,位置,回転,拡縮を設定する
	mpModel = model;  //モデルの設定
	mPosition = position;  //位置の設定
	mRotation = rotation;  //回転の設定
	mScale = scale;  //拡縮の設定
}