#include "CEnemy.h"
#include"CEffect.h"
#include "CCollisionManager.h"
#include"CPlayer.h"
#include "CExclamationMark.h"
#include"CCharacter3.h"
#include"CCollider.h"
#include"CTaskManager.h"

//移動速度
#define VELOCITY CVector(0.0f,0.0f,0.1f)//9
#define VELOCITY2 0.09f
#define VELOCITY3 0.02f
#define ROTATION_Y CVector(0.0f,-0.1f,0.0f) //回転速度

CModel CEnemy::sModel;
//
CEnemy* CEnemy::Instance()
{
	return spInstance;
}

CEnemy* CEnemy::spInstance = nullptr;

CEnemy::CEnemy()
	:CCharacter3(1)
	//, mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 0.4f)
	,mark(0)
	,mark2(0)
{
	//モデルが無いときは読み込む
	if (sModel.Triangles().size() == 0)
	{
		//sModel.Load(OBJ, MTL);
	}
	//モデルのポインタ
	mpModel = &sModel;
	spInstance = this;
}

//コンストラクタ
//CEnemy(位置,回転,拡縮)
CEnemy::CEnemy(const CVector& position, const CVector& rotation, const CVector& scale)
	:CEnemy()  //デフォルトコンストラクタを実行する
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
void CEnemy::Update()
{
	//プレイヤーのポインタが0以外の時
	CPlayer* player = CPlayer::Instance();
	if (player != nullptr)
	{
		//プレイヤーまでのベクトルを求める
		CVector vp = player->Position() - mPosition;
		//左ベクトルとの内積を求める
		float dx = vp.Dot(mMatrixRotate.VectorX());
		//上ベクトルとの内積を求める
		float dy = vp.Dot(mMatrixRotate.VectorY());
		float dz = vp.Dot(mMatrixRotate.VectorZ());

		//X軸のズレが2.0以下
		if (-10.0f < dx && dx < 10.0f)
		{
		    mRotation = mRotation + CVector(0.0f, 1.0f, 0.0f);
			//mPosition = mPosition + mMatrixRotate.VectorZ() * VELOCITY3;
			//Y軸のズレが2.0以下
			if (-10.0f < dy && dy < 10.0f)
			{
				//mPosition = mPosition + mMatrixRotate.VectorZ() * VELOCITY2;
				if (20.0f > dz  && dz > -5.0f)
				{
					mState = EState::EMOVE;
					if (mState == EState::EMOVE)
					{
						mPosition = mPosition + mMatrixRotate.VectorZ() * VELOCITY2;
					}
					/*CExclamationMark* exclamationmark = new CExclamationMark();
					exclamationmark->Set(0.0f, 0.0f);
					exclamationmark->Position(
						CVector(0.8f, 5.0f, 0.0f) * mMatrix);
					exclamationmark->Rotation(mRotation);
					exclamationmark->Update();
					mark = 0;*/

					//弾を発射します
					/*CBullet* bullet = new CBullet();
					bullet->Set(0.1f, 1.5f);
					bullet->Position(
						CVector(0.0f, 0.0f, 10.0f) * mMatrix);
					bullet->Rotation(mRotation);
					bullet->Update();*/
				}
				else //if (5.0f == dz || dz == -1.0f)
				{
					mState = EState::ESTOP;
					mRotation = mRotation + CVector(0.0f, 1.0f, 0.0f);
					mPosition = mPosition + mMatrixRotate.VectorZ() * VELOCITY2;
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
		mRotation = mRotation + CVector(0.0f, 1.0f, 0.0f);  //左へ回転
		//mPosition = mPosition + mMatrixRotate.VectorZ() * VELOCITY3;
	}
	else if (dx < -margin)
	{
		//課題
		mRotation = mRotation + CVector(0.0f, -1.0f, 0.0f);  //右へ回転
		//mPosition = mPosition + mMatrixRotate.VectorZ() * VELOCITY3;
		//mPosition = mPosition + mMatrixRotate.VectorX() * VELOCITY2;
	}
	//上下方向へ回転
	if (dy > margin)
	{
		//mRotation = mRotation + CVector(-1.0f, 0.0f, 0.0f);  //上へ回転
		//mPosition = mPosition + mMatrixRotate.VectorZ() * VELOCITY2;
	}
	else if (dx < -margin)
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
	if (r == 0)
	{
		if (player != nullptr)
		{
			mPoint = player->Position();
		}
		else
		{
			mPoint = mPoint * CMatrix().RotateY(45);
		}
	}
	////行列を更新
	CTransform::Update();
	////位置を移動
	//mPosition = mPosition + VELOCITY * mMatrixRotate;
}

void CEnemy::Collision()
{
	//mColliderMesh1.ChangePriority();
	//コライダの優先度変更
	mCollider.ChangePriority();
	mLine.ChangePriority();
	mLine2.ChangePriority();
	mLine3.ChangePriority();
	CCollisionManager::Instance()->Collision(&mCollider, COLLISIONRANGE);
	/*CCollisionManager::Instance()->Collision(&mLine, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mLine2, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mLine3, COLLISIONRANGE);*/
	////衝突処理を実行
	//CCollisionManager::Instance()->Collision(&mCollider, COLLISIONRANGE);
	/*CCollisionManager::Instance()->Collision(&mCollider2, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mCollider3, COLLISIONRANGE);*/
}

//衝突処理
//Collision(コライダ1,コライダ2)
void CEnemy::Collision(CCollider * m, CCollider * o) {
	//相手のコライダタイプの判定
	switch (m->ELINE)
	{
	//case CCollider::ESPHERE: //球コライダの時
	//	CVector adjust;//調整用ベクトル
	//	//コライダのmとyが衝突しているか判定
	//	if (CCollider::CCollision(m, o)) 
	//	{
	//		mPosition = mPosition + adjust;
	//		//CTransform::Update();
	//		//エフェクト生成
	//		//new CEffect(o->Parent()->Position(), 1.0f, 1.0f, "exp.tga", 4, 4, 2);
	//		//衝突している時は無効にする
	//		//mEnabled=false;
	//	}
	//	break;
	case CCollider::ELINE: //三角コライダの時
		//三角コライダと球コライダの衝突判定
		if (o->Type() == CCollider::ETRIANGLE)
		{
			CVector adjust;//調整用ベクトル
			if (CCollider::CollisionTriangleSphere(o, m, &adjust))
			{
				mState = EState::ESTOP;
				//mState = EState::ESTOP;
				//衝突しない位置まで戻す
				mPosition = mPosition + adjust;
				if (rand() % 60)
				{
					mState = EState::ESTOP;
				}
				else
				{
					mState = EState::ESTOP;
				}
				CTransform::Update();
			}
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
//CEnemy(モデル,位置,回転,拡縮)
CEnemy::CEnemy(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale)
: mark(0)
,mark2(0)
,mCollider(this, &mMatrix, CVector(0.0f, 2.5f, 0.0f), 1.0f)
,mLine(this, &mMatrix, CVector(0.0f, 4.9f, 0.0f), CVector(0.0f, 0.0f, 0.0f))
, mLine2(this, &mMatrix, CVector(1.0f, 2.5f, -0.5f), CVector(-1.0f, 2.5f, -0.5f))
, mLine3(this, &mMatrix, CVector(0.0f, 2.5f, 0.5f), CVector(0.0f, 2.5f, -0.5f))
	//, mCollider2(this, &mMatrix, CVector(0.0f,1.0f,5.0f),0.8f)
   // , mCollider3(this, &mMatrix, CVector(0.0f, 1.0f, -5.0f), 0.8f)
{
	CTransform::Update(position, rotation, scale);
	//モデル,位置,回転,拡縮を設定する
	mpModel = model;  //モデルの設定
	mPosition = position;  //位置の設定
	mRotation = rotation;  //回転の設定
	mScale = scale;  //拡縮の設定
	spInstance = this;
	//mColliderMesh1.Set(this, &mMatrix, mpModel);
}