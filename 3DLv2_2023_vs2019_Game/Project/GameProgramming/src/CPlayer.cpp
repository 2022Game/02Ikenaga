//プレイヤークラスのインクルード
#include "CPlayer.h"
#include"CTaskManager.h"
#include"CApplication.h"
#include"CCharacter3.h"
#define ROTATION_YV CVector(0.0f,1.0f,0.0f) //回転速度
#define ROTATION_YY CVector(0.0f,2.0f,0.0f) //回転速度
#define ROTATION_YO CVector(0.0f,0.3f,0.0f) 
#define ROTATION_Y CVector(0.0f,-0.1f,0.0f) //回転速度
#define ROTATION_X CVector(0.0f,-0.1f,0.0f) //回転速度
#define VELOCITY CVector(0.0f,0.0f,0.09f)//移動速度
#define  ROTATION_XV CVector(1.0f,0.0f,0.f) //回転速度
#define HP 1 //HP
//#define ZYUURYOKU (TIPSIZE/20.0f)
//#define JANPU (TIPSIZE /3.0f)

int CPlayer::sHp = 0;

int CPlayer::Hp()
{
	return sHp;
}

CPlayer* CPlayer::Instance()
{
	return spInstance;
}

CPlayer* CPlayer::spInstance = nullptr;

//衝突処理
void CPlayer::Collision()
{
	mCollider.ChangePriority();
	//衝突処理を実行
	//コライダの優先度変更
	mLine.ChangePriority();
	mLine2.ChangePriority();
	mLine3.ChangePriority();
	//衝突処理を実行
	CCollisionManager::Instance()->Collision(&mCollider, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mLine, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mLine2, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mLine3, COLLISIONRANGE);
}

void CPlayer::Collision(CCollider* m, CCollider* o) {
	//自身のコライダタイプの判定
	switch (m->Parent()->Tag())
	{
	case CCollider::ESPHERE: //球コライダの時
			//コライダのmとoが衝突しているか判定
		if (CCollider::CCollision(m, o))
		{
			sHp = 0;
			//衝突している時は無効にする
			//mEnabled=false;
			CTransform::Update();
		}
		//break;
	case CCollider::ELINE://線分コライダ
		//相手のコライダが三角コライダの時
		if (o->Type() == CCollider::ETRIANGLE)
		{
			CVector adjust;//調整用ベクトル
			//三角形と線分の衝突判定
			if (CCollider::CollisionTriangleLine(o, m, &adjust))
			{
				//位置の更新(mPosition+adjust)
				mPosition = mPosition + adjust;
				//sHp=0;
				//行列の更新
				CTransform::Update();
			}
		}
		break;
	}
}

CPlayer::CPlayer()
	:mLine(this, &mMatrix, CVector(0.0f, 1.9f,0.0f), CVector(0.0f, 0.0f, 0.0f))
	,mLine2(this, &mMatrix, CVector(0.3f, 1.0f, 1.0f), CVector(-0.3f, 1.0f, 1.0f))
	,mLine3(this, &mMatrix, CVector(0.0f, 1.0f, 0.5f), CVector(0.0f, 1.0f, -0.5f))
	,jump(0)
	, mCollider(this, &mMatrix, CVector(0.0f, 1.0f, 0.0f), 0.5f)
	//, mLine4(this, &mMatrix, CVector(0.0f, 1.0f, 2.5f), CVector(0.0f, 1.0f, -0.5f))
{
	ETag::EPLAYER;
	sHp = HP;

	mpModel = new CModel();
	mpModel->Load("res\\obama4.obj", "res\\obama4.mtl");
	//インスタンスの設定
	spInstance = this;
}

//CPayer(位置,回転,スケール)
CPlayer::CPlayer(const CVector& pos, const CVector& rot, const CVector& scale)
	:jump(0)
{
	CTransform::Update(pos, rot, scale); //行列の更新
}

//更新処理
void CPlayer::Update()
{
	SetHidden(false);
	if (mInput.Key('Y')) 
	{
		SetHidden(true);
	}
	//スペースキー入力で弾発射
	if (mInput.Key(VK_RBUTTON))//右クリック//LBUTTON左クリック
	{
		CBullet* bullet = new CBullet();
		bullet->Set(0.1f, 1.5);
		bullet->Position(CVector(0.0f, 0.0f, 10.0f) * mMatrix);
		bullet->Rotation(mRotation);
		bullet->Update();
		//CApplication::TaskManager()->Add(bullet);
	}
	//Dキー入力で回転
	if (mInput.Key('D')) {
		//Y軸の回転値を減少
		mRotation = mRotation - ROTATION_YV;
	}
	//Aキー入力で回転
	if (mInput.Key('A')) {
		//Y軸の回転値を減少
		mRotation = mRotation + ROTATION_YV;
	}
	//上キー入力で前進
	if (mInput.Key(VK_UP)) {
		//Z軸方向の値を回転させ移動させる
		mPosition = mPosition + VELOCITY * mMatrixRotate;
   }
	//Sキー入力で上向き
	if (mInput.Key('S')) {
		//X軸の回転値を減算
		//mRotation = mRotation - ROTATION_XV;
		mPosition = mPosition - VELOCITY * mMatrixRotate;
	}
	//Wキー入力で上向き
	if (mInput.Key('W')) {
		//X軸の回転値を加算
		//mRotation = mRotation + ROTATION_XV;
		mPosition = mPosition + VELOCITY * mMatrixRotate;
		//mPosition = mPosition + ROTATION_X * mMatrixRotate;
	}
	//if (mState != EState::EJUMP)
	//{
	//	if (mInput.Key('J'))
	//	{
	//		//mPosition = mPosition + ROTATION_YY * mMatrixRotate;
	//	}
	//}
	jump++;
	if (jump > 50)
	{
		if (mInput.Key('J'))
		{
			//mPosition = mPosition + ROTATION_Y * mMatrixRotate;
			mPosition = mPosition + ROTATION_YY * mMatrixRotate;
			jump = 0;
		}
	}
	//if (mInput.Key('J'))
	//{
	//	//mPosition = mPosition + ROTATION_Y * mMatrixRotate;
	//	mPosition = mPosition + ROTATION_YO * mMatrixRotate;
	//}
	mPosition = mPosition + ROTATION_Y * mMatrixRotate;
	//変換行列の更新
	CTransform::Update();

	//UI設定
	CApplication::Ui()->PosY(mPosition.Y());
	CApplication::Ui()->RotX(mRotation.X());
	CApplication::Ui()->RotY(mRotation.Y());
	/*CStageOne::Ui()->PosY(mPosition.Y());
	CStageOne::Ui()->RotX(mRotation.X());
	CStageOne::Ui()->RotY(mRotation.Y());*/
}