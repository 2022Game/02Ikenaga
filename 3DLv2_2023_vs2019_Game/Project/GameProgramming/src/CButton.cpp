#include "CButton.h"
#include"CStageManager.h"
#include "CTask.h"
#include "CCollisionManager.h"
#include "CPlayer.h"

#define VELOCITYX -0.02

CButton* CButton::Instance()
{
	return spInstance;
}

CButton* CButton::spInstance = nullptr;

CButton::CButton()
{
	spInstance = this;
}

void CButton::Collision()
{
	mColliderMesh1.ChangePriority();
	mLine2.ChangePriority();
	mLine3.ChangePriority();
	mLine.ChangePriority();
}

void CButton::Collision(CCollider* m, CCollider* o) {
	//相手のコライダタイプの判定
	switch (o->ETRIANGLE)
	{
	//case CCollider::ESPHERE: //球コライダの時
	//	if (CCollider::CCollision(o, m))
	//	{
	//	}
	//	break;
	case CCollider::ETRIANGLE:
		//三角コライダと球コライダの衝突判定
		if (o->Type() == CCollider::ETRIANGLE)
		{
			CVector adjust;//調整用ベクトル
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

CButton::CButton(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale)
	//:mCollider(this, &mMatrix, CVector(0.0f, 0.5f, 0.0f), 0.5f)
	/*:mLine(this, &mMatrix, CVector(0.0f, 1.0f, 0.0f), CVector(0.0f, 0.0f, 0.0f))
	, mLine2(this, &mMatrix, CVector(0.5f, 0.5f, 0.0f), CVector(-0.5f, 0.5f, 0.0f))
	, mLine3(this, &mMatrix, CVector(0.0f, 0.5f, 0.5f), CVector(0.0f, 0.5f, -0.5f))*/
{
	spInstance = this;
	//モデル,位置,回転,拡縮を設定する
	mpModel = model;  //モデルの設定
	mPosition = position;  //位置の設定
	mRotation = rotation;  //回転の設定
	mScale = scale;  //拡縮の設定
	mColliderMesh1.Set(this, &mMatrix, mpModel);
}

//更新処理
void CButton::Update()
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

		if (dx < 0.1f && dy < 10.0f && dz < 0.5f && dz>-0.5f)
		{
			SetHidden(true);
			mPosition = mPosition + mMatrixRotate.VectorZ() * VELOCITYX;
		}
		//if  (dy < 0.1f && dz < -0.5f)//(dx < 0.5f && dy < 0.1f && dz == 10.1f)
		//{
		//	SetHidden(true);
		//	mPosition = mPosition + mMatrixRotate.VectorZ() * VELOCITY;
		//}
		else
		{
			//SetHidden(false);
		}
	}
	// 行列を更新
	CTransform::Update();
}