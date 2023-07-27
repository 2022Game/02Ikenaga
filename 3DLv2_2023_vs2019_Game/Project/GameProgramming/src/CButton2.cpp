#include "CButton2.h"
#include "CButton.h"
#include "CCollisionManager.h"
#include"CPlayer.h"

CModel CButton2::sModel;

//移動速度
#define VELOCITY CVector(0.0f,0.0f,0.1f)
#define VELOCITYYX CVector(0.0f,-0.02f,0.0f)
#define VELOCITYYZ -0.02f

void CButton2::Set(float w, float d)
{
	//スケール設定
	mScale = CVector(0.01f, 0.01f, 0.01f);
}

CButton2::CButton2()
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

//コンストラクタ
//CButton2(位置,回転,拡縮)
CButton2::CButton2(const CVector& position, const CVector& rotation, const CVector& scale)
	:CButton2()  //デフォルトコンストラクタを実行する
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
void CButton2::Update()
{
	SetHidden(true);
	CButton* button = CButton::Instance();
	if (button != nullptr)
	{
		//プレイヤーまでのベクトルを求める
		CVector vp = button->Position() - mPosition;
		//左ベクトルとの内積を求める
		float dx = vp.Dot(mMatrixRotate.VectorX());
		//上ベクトルとの内積を求める
		float dy = vp.Dot(mMatrixRotate.VectorY());
		float dz = vp.Dot(mMatrixRotate.VectorZ());

		if (dy > 1.0f)
		{
			CTransform::Update();
			mPosition = mPosition + mMatrixRotate.VectorZ() * VELOCITYYZ;
		}
	}
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
	int r = rand() % 60; //rand()は整数の乱数を返す
						  //% 180は180で割った余りを求める
						  // 
	//行列を更新
	CTransform::Update();
	//位置を移動
	//mPosition = mPosition + VELOCITY * mMatrixRotate;
}

void CButton2::Collision()
{
	mColliderMesh1.ChangePriority();
}

//衝突処理
//Collision(コライダ1,コライダ2)
void CButton2::Collision(CCollider* m, CCollider* o) {
	//相手のコライダタイプの判定
	switch (o->Type())
	{
	case CCollider::ELINE: //三角コライダの時
		CVector adjust; //調整値
		//三角コライダと球コライダの衝突判定
		if (CCollider::CollisionTriangleSphere(o, m, &adjust))
		{
			//衝突しない位置まで戻す
			mPosition = mPosition + adjust;
			CTransform::Update();
		}
		break;
	}
}

//コンストラクタ
//CExclamationMark(モデル,位置,回転,拡縮)
CButton2::CButton2(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale)
{
	//モデル,位置,回転,拡縮を設定する
	mpModel = model;  //モデルの設定
	mPosition = position;  //位置の設定
	mRotation = rotation;  //回転の設定
	mScale = scale;  //拡縮の設定
	mColliderMesh1.Set(this, &mMatrix, mpModel);
}