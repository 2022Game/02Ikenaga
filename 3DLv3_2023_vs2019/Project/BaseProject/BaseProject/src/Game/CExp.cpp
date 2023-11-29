#include "CExp.h"
#include "CCollisionManager.h"

CExp::CExp()
	: CBillBoardImage("Item\\exp.png",ETag::eExp, ETaskPriority::eExp, 
		0, ETaskPauseType::eGame)
	,mMoveSpeed(0.0f,0.0f,0.0f)
{
	mpColliderSphere = new CColliderSphere
	(
		this,ELayer::eExp,
		1.1f
	);
	mpColliderSphere->SetCollisionLayers({ ELayer::ePlayer,ELayer::eField });
	//mpColliderSphere->Position(-0.03f, 0.1f, 0.05f);
}

CExp::~CExp()
{
	SAFE_DELETE(mpColliderSphere);
}


// 初期設定
void CExp::Setup(const CVector& pos, const CVector& dir)
{
	Position(pos);

	//　移動方向ベクトルを横軸回転して上方向へ向ける
	CVector side = CVector::Cross(CVector::up, dir);
	CQuaternion q;
	q.RotationAxis(side, 75.0f);
	mMoveSpeed = q* dir * 75.0f;
}

void CExp::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (self == mpColliderSphere)
	{
		// プレイヤーと衝突した場合
		if (other->Layer() == ELayer::ePlayer)
		{
			Kill();
		}
		// フィールドと衝突した場合
		else if (other->Layer() == ELayer::eField)
		{
			//押し戻す
			Position(Position() + hit.adjust * hit.weight);

			// 移動ベクトルを反射させる
			CVector moveVec = mMoveSpeed;
			CVector n = hit.adjust.Normalized();
			float d = CVector::Dot(n, moveVec);
			CVector reflect = moveVec - (n * d * 2.0f);
			mMoveSpeed = reflect * 0.7f;

			// Y軸の移動量を0にする
			//mMoveSpeed.Y(0.0f);
		}
	}
}

void CExp::Update()
{
	// 重力を加算
	mMoveSpeed.Y(mMoveSpeed.Y() - 1.0f);
	// 座標を移動速度分移動させる
	Position(Position() + mMoveSpeed * Time::DeltaTime());
}