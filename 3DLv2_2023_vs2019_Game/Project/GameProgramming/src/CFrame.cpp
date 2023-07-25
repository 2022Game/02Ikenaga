#include "CFrame.h"
#include "CCollisionManager.h"
#include"CPlayer.h"

CModel CFrame::sModel;

//移動速度
#define VELOCITY CVector(0.0f,0.0f,0.1f)//9

void CFrame::Set(float w, float d)
{
	//スケール設定
	mScale = CVector(0.2f, 0.2f, 0.2f);
	//三角形の頂点設定
	//mT.Vertex(CVector(w, 0.0f, 0.0f), CVector(0.0f, 0.0f, -d), CVector(-w, 0.0f, 0.0f));
	//三角形の法線設定
	//mT.Normal(CVector(0.0f, 1.0f, 0.0f));
}

CFrame::CFrame()
	:CCharacter3(1)
{
	//モデルが無いときは読み込む
	if (sModel.Triangles().size() == 0)
	{
		//sModel.Load(OBJ1, MTL1);
	}
	//モデルのポインタ
	mpModel = &sModel;
}

// 更新処理
void CFrame::Update()
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
}

void CFrame::Collision()
{
}

//衝突処理
//Collision(コライダ1,コライダ2)
void CFrame::Collision(CCollider* m, CCollider* o) {
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
		}
		break;
	}
}

//コンストラクタ
//CExclamationMark(モデル,位置,回転,拡縮)
CFrame::CFrame(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale)
{
	//モデル,位置,回転,拡縮を設定する
	mpModel = model;  //モデルの設定
	mPosition = position;  //位置の設定
	mRotation = rotation;  //回転の設定
	mScale = scale;  //拡縮の設定
	mPoint = mPosition + CVector(0.0f, 0.0f, 10.0f) * mMatrixRotate;
}