#include "CExp.h"
#include "CCollisionManager.h"
#include "CPlayer.h"

CExp::CExp()
	: CBillBoardImage("Item\\exp.png",ETag::eExp, ETaskPriority::eExp, 
		0, ETaskPauseType::eGame)
	,mMoveSpeed(0.0f,0.0f,0.0f)
	, mState(EState::Drop)
	, mElapsedTime(0.0f)
{
	mpColliderSphere = new CColliderSphere
	(
		this,ELayer::eExp,
		0.5f
	);
	mpColliderSphere->SetCollisionLayers({ ELayer::ePlayer, ELayer::eField });
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
		//else if(other->Layer() == ELayer::ePlayer)
		//{
		//	mpColliderSphere->SetEnable(true);
		//}
		//else
		//{
		//	// 吸収状態では地面との当たり判定は無効
		//	//mpColliderSphere->SetEnable(false);
		//}
	}
}

void CExp::Update()
{
	// プレイヤーの情報を取得
	CPlayer* player = CPlayer::Instance();
	// プレイヤーの座標
	CVector playerPos = CVector::zero;
	// プレイヤーまでのベクトル
	CVector playerVec = CVector::zero;
	// プレイヤーまでの距離
	float dist = 9999.0f;
	if (player != nullptr)
	{
		playerPos = player->Position() + CVector(0.0f, 5.0f, 0.0f);
		playerVec = playerPos - Position();
		dist = playerVec.Length();
	}
	CDebugPrint::Print(" 経験値距離:%f\n", dist);


	// 状態に合わせて移動処理を切り替える
	switch (mState)
	{
		// 経験値を落とした時の処理
	    case EState::Drop:
			// 経験値を吸収する範囲内もしくは、
			// 一定時間が経過していたら、吸収状態へ移行
			if (mElapsedTime >= 1.0f && dist <= 20.0f
				|| mElapsedTime>=3.0f)
			{
				mState = EState::Absorb;
				mMoveSpeed = CVector::zero;
				// 吸収状態では地面との当たり判定は無効
				//mpColliderSphere->SetEnable(false);
			}
			else
			{
				// 重力を加算
				mMoveSpeed.Y(mMoveSpeed.Y() - 2.0f);
				// 座標を移動速度分移動させる
				Position(Position() + mMoveSpeed * Time::DeltaTime());
			}
			// 経過時間を計測
			mElapsedTime += Time::DeltaTime();
			break;
		// 経験値が吸収される時の処理
		case EState::Absorb:
			// 加速度
			float accelSpeed = 200.0f * Time::DeltaTime();
			// 現在の移動速度 + 加速度 で現在の移動速度を計算
			float moveSpeed = mMoveSpeed.Length() + accelSpeed;

			// プレイヤーの方向への移動ベクトルを計算
			mMoveSpeed = playerVec.Normalized() * moveSpeed;
			CVector currMoveSpeed = mMoveSpeed * Time::DeltaTime();
			// プレイヤーまでの距離が移動距離より短いならば
			if (dist <= currMoveSpeed.Length())
			{
				// 経験値を取得
				
				// 経験値は削除
				//Kill();
			}
			else
			{
				// プレイヤーの方向へ移動
				Position(Position() + currMoveSpeed);
			}
			break;
	}
}