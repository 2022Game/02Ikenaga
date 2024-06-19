#include "CHomingBallEffect.h"
#include "CHomingBall.h"
#include "Maths.h"
#include "CCharaBase.h"

// ホーミングボールの発射の間隔時間
#define THROW_INTERVAL 0.4f
// ホーミングボールの発射の方向のブレ幅
#define DIR_RAND 0.01f
// ホーミングボールの色
#define COLOR CColor(1.0f, 1.0f, 0.0f)

// コンストラクタ
CHomingBallEffect::CHomingBallEffect(CObjectBase* owner, const CMatrix* attach,
	const CVector& offsetPos, const CMatrix& offsetRot)
	: mpOwner(owner)
	, mpAttachMtx(attach)
	, mThrowOffsetPos(offsetPos)
	, mThrowOffsetRot(offsetRot)
	, mElapsedTime(0.0f)
	, mIsThrowing(false)
{
}

// デストラクタ
CHomingBallEffect::~CHomingBallEffect()
{
}

// ホーミングボールの発射を開始
void CHomingBallEffect::Start()
{
	mIsThrowing = true;
	mElapsedTime = 0.0f;
}

// ホーミングボールの発射を停止
void CHomingBallEffect::Stop()
{
	mIsThrowing = false;
}

// ホーミングボールをしているかどうか
bool CHomingBallEffect::IsThrowing() const
{
	return mIsThrowing;
}

// ホーミングボールの発射時のオフセット位置を設定
void CHomingBallEffect::SetThrowOffsetPos(const CVector& pos)
{
	mThrowOffsetPos = pos;
}

// ホーミングボールの発射時のオフセット回転値を設定
void CHomingBallEffect::SetThrowOffsetRot(const CMatrix& rot)
{
	mThrowOffsetRot = rot;
}

// ホーミングボールの発射の位置を取得
CVector CHomingBallEffect::GetThrowPos() const
{
	// アタッチする行列が設定されている場合は、行列の座標を返す
	if (mpAttachMtx != nullptr)
	{
		CVector pos = mpAttachMtx->Position();
		pos += mpAttachMtx->VectorX().Normalized() * mThrowOffsetPos.X()
			+ mpAttachMtx->VectorY().Normalized() * mThrowOffsetPos.Y()
			+ mpAttachMtx->VectorZ().Normalized() * mThrowOffsetPos.Z();
		return pos;
	}
	// 持ち主が設定されている場合は、持ち主の座標を返す
	else if (mpOwner != nullptr)
	{
		return mpOwner->Position() + (mpOwner->Rotation() * mThrowOffsetPos);
	}

	return CVector::zero;
}

// ホーミングボールの発射の方向を取得
CVector CHomingBallEffect::GetThrowDir() const
{
	// アタッチする行列が設定されている場合は、行列の正面方向ベクトルを返す
	if (mpAttachMtx != nullptr)
	{
		return mThrowOffsetRot * mpAttachMtx->VectorZ();
	}
	// 持ち主が設定されている場合は、持ち主の正面方向ベクトルを返す
	else if (mpOwner != nullptr)
	{
		return mThrowOffsetRot * mpOwner->VectorZ();
	}

	return CVector::forward;
}

// ホーミングボールを作成
void CHomingBallEffect::CreateLightningBall()
{
	// ホーミングボールを作成
	CHomingBall* homingBall = new CHomingBall(ETag::eHomingBall);
	CCharaBase* owner = dynamic_cast<CCharaBase*>(mpOwner);
	if (owner != nullptr)
	{
		homingBall->SetOwner(owner);
	}

	// 位置を取得
	CVector pos = GetThrowPos();// + CVector(0.0f, 10.0f, 0.0f);
	// 方向を取得
	CVector dir = GetThrowDir();// + CVector(0.0f, -1.0f, 0.0f);
	// 方向をランダムでブラす
	dir.X(dir.X() + Math::Rand(-DIR_RAND, DIR_RAND));
	dir.Y(dir.Y() + Math::Rand(-DIR_RAND, DIR_RAND));
	dir.Z(dir.Z() + Math::Rand(-DIR_RAND, DIR_RAND));
	dir.Normalize();
	// 位置、方向、移動速度を設定
	homingBall->Setup(pos, dir, 50.0f);

	// ホーミングボールのカラーを設定
	homingBall->SetColor(COLOR);
	// 加算ブレンドにして、ホーミングボールが発光しているように見せる
	homingBall->SetBlendType(EBlend::eAdd);
}

// 更新
void CHomingBallEffect::Update()
{
	// ホーミングボールを発射していたら
	if (mIsThrowing)
	{
		// 経過時間に応じて、ホーミングボールを作成
		if (mElapsedTime >= THROW_INTERVAL)
		{
			CreateLightningBall();
			mElapsedTime -= THROW_INTERVAL;
		}
		mElapsedTime += Time::DeltaTime();
	}
}