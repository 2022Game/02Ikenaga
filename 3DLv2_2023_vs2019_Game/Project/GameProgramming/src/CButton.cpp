#include "CButton.h"
#include"CStageManager.h"
#include "CCollisionManager.h"
#include "CPlayer.h"
#include "CResourceManager.h"
#include "CSceneBase.h"
#include "CMeat.h"

#define VELOCITYX -0.02
#define ROTATION_YY CVector(0.0f,0.09f,0.0f)
#define ROTATION_YZ CVector(0.0f,-0.02f,0.0f)

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
}

void CButton::Collision(CCollider* m, CCollider* o) {
	//相手のコライダタイプの判定
	switch (o->Type())
	{
	//case CCollider::ESPHERE: //球コライダの時
	//	if (CCollider::CCollision(o, m))
	//	{
	//	}
	//	break;
	case CCollider::ELINE:
		//三角コライダと球コライダの衝突判定
		if (o->Type() == CCollider::ELINE)
		{
			CVector adjust;//調整用ベクトル
			if (CCollider::CollisionTriangleSphere(o, m, &adjust))
			{
				SetHidden(true);
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

		if (dx < 0.1f && dy < 1.0f && dz < 0.5f && dz>-0.5f)
		{
			CTransform::Update();
			mPosition = mPosition + ROTATION_YY * mMatrixRotate;
		}
	}
	// 行列を更新
	CTransform::Update();
}