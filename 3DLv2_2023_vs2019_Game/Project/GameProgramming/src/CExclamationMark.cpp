#include "CExclamationMark.h"
#include"CEffect.h"
#include "CCollisionManager.h"
#include"CPlayer.h"
#include "CEnemy.h"

#define OBJ1 "res\\!.obj"
#define MTL1 "res\\!.mtl"

CModel CExclamationMark::sModel;

//移動速度
#define VELOCITY CVector(0.0f,0.0f,0.1f)//9
#define VELOCITY2 0.11f
#define VELOCITY3 -0.11f

void CExclamationMark::Set(float w, float d)
{
	//スケール設定
	mScale = CVector(0.2f, 0.2f,0.2f);
	//三角形の頂点設定
	//mT.Vertex(CVector(w, 0.0f, 0.0f), CVector(0.0f, 0.0f, -d), CVector(-w, 0.0f, 0.0f));
	//三角形の法線設定
	//mT.Normal(CVector(0.0f, 1.0f, 0.0f));
}

CExclamationMark::CExclamationMark()
	:CCharacter3(1)
	,mark(0)
	//, mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 0.4f)
{
	//モデルが無いときは読み込む
	if (sModel.Triangles().size() == 0)
	{
		sModel.Load(OBJ1, MTL1);
	}
	//モデルのポインタ
	mpModel = &sModel;
}

//コンストラクタ
//CExclamationMark(位置,回転,拡縮)
CExclamationMark::CExclamationMark(const CVector& position, const CVector& rotation, const CVector& scale)
	:CExclamationMark()  //デフォルトコンストラクタを実行する
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
void CExclamationMark::Update()
{
	/*mark++;
	if (mark > 1)
	{
		mEnabled = false;
		mark = 0;
	}*/
		//プレイヤーのポインタが0以外の時
		//CPlayer* player = CPlayer::Instance();
		//if (player != nullptr)
		//{
		//	//プレイヤーまでのベクトルを求める
		//	CVector vp = player->Position() - mPosition;
		//	//左ベクトルとの内積を求める
		//	float dx = vp.Dot(mMatrixRotate.VectorX());
		//	//上ベクトルとの内積を求める
		//	float dy = vp.Dot(mMatrixRotate.VectorY());
		//	float dz = vp.Dot(mMatrixRotate.VectorZ());
	CEnemy* enemy = CEnemy::Instance();
	if (enemy != nullptr)
	{
		CVector vp = enemy->Position() - mPosition;
		//左ベクトルとの内積を求める
		float dx = vp.Dot(mMatrixRotate.VectorX());
		//上ベクトルとの内積を求める
		float dy = vp.Dot(mMatrixRotate.VectorY());
		float dz = vp.Dot(mMatrixRotate.VectorZ());

			//X軸のズレが2.0以下
		if  (-100.0f < dx && dx < 100.0f)
		{
			mPosition = mPosition + mMatrixRotate.VectorZ() * VELOCITY2;
			//Y軸のズレが2.0以下
			if (-1.0f < dy && dy < 1.0f)
			{
				//mPosition = mPosition + mMatrixRotate.VectorZ() * VELOCITY2;
				if (30.0f > dz && dz >= 0.0f)
				{
					//弾を発射します
					/*CBullet* bullet = new CBullet();
					bullet->Set(0.1f, 1.5f);
					bullet->Position(
						CVector(0.0f, 0.0f, 10.0f) * mMatrix);
					bullet->Rotation(mRotation);
					bullet->Update();*/
				}
			}
		}
	}
	//HPが0以下の時 撃破
	//if (mHp <= 0)
	//{
	//	mHp--;
	//	//15フレーム毎にエフェクト
	//	if (mHp % 15 == 0)
	//	{
	//		//エフェクト生成
	//		new CEffect(mPosition, 1.0f, 1.0f, "exp.tga", 4, 4, 2);
	//	}
	//	//下降させる
	//	mPosition = mPosition - CVector(0.0f, 0.03f, 0.0f);
	//	CTransform::Update();
	//	return;
	//}
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
		mPosition = mPosition + mMatrixRotate.VectorZ() * VELOCITY2;
	}
	else if (dy < -margin)
	{
		//課題
		//mRotation = mRotation + CVector(1.0f, 0.0f, 0.0f);  //下へ回転
	  mPosition = mPosition + mMatrixRotate.VectorZ() * VELOCITY2;
	}
	//機体前方へ移動する
	//mPosition = mPosition + mMatrixRotate.VectorZ() * VELOCITY2;
	CTransform::Update(); //行列更新
	//およそ3秒毎に目標地点を更新
	int r = rand() % 180; //rand()は整数の乱数を返す
						  //% 180は180で割った余りを求める
	if (r == 0)
	{
		if (enemy == nullptr)
		{
			mPoint = enemy->Position();
		}
		else
		{
			mPoint = mPoint * CMatrix().RotateY(45);
		}
	}

	/*if (r == 0)
	{
		if (player != nullptr)
		{
			mPoint = player->Position();
		}
		else
		{
			mPoint = mPoint * CMatrix().RotateY(45);
		}
	}*/
	//行列を更新
	//CTransform::Update();
	//位置を移動
	//mPosition = mPosition + VELOCITY * mMatrixRotate;
}

void CExclamationMark::Collision()
{
	//コライダの優先度変更
	mCollider1.ChangePriority();
	mCollider2.ChangePriority();
	mCollider3.ChangePriority();
	//衝突処理を実行
	CCollisionManager::Instance()->Collision(&mCollider1, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mCollider2, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mCollider3, COLLISIONRANGE);
}

//衝突処理
//Collision(コライダ1,コライダ2)
void CExclamationMark::Collision(CCollider* m, CCollider* o) {
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
			//衝突しない位置まで戻す
			//mPosition = mPosition + adjust;
		}
		break;
	}

	////コライダもmとoが衝突しているか判定
	//if (CCollider::CCollision(m, o)) {
	//	//エフェクト生成
	//	new CEffect(o->Parent()->Position(), 1.0f, 1.0f, "exp.tga", 4, 4, 2);
	//	//mEnabled = false;
	//}
}

//コンストラクタ
//CExclamationMark(モデル,位置,回転,拡縮)
CExclamationMark::CExclamationMark(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale)
	:mark(0)
/*: mCollider1(this, &mMatrix, CVector(0.0f, 5.0f, 0.0f), 0.8f)
, mCollider2(this, &mMatrix, CVector(0.0f,5.0f,20.0f),0.8f)
, mCollider3(this, &mMatrix, CVector(0.0f, 5.0f, -20.0f), 0.8f)*/
{
	//モデル,位置,回転,拡縮を設定する
	mpModel = model;  //モデルの設定
	mPosition = position;  //位置の設定
	mRotation = rotation;  //回転の設定
	mScale = scale;  //拡縮の設定
	//mColliderMesh1.Set(this, &mMatrix, mpModel);
}