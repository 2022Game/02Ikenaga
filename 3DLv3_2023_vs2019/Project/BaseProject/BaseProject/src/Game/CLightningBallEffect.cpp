#include "CLightningBallEffect.h"
#include "CLightningBall.h"
#include "Maths.h"

CLightningBallEffect* CLightningBallEffect::spInstance = nullptr;

// 雷球発射の間隔時間
#define THROW_INTERVAL 0.4f
// 雷球発射の方向のブレ幅
#define FLAME_DIR_RAND 0.02f
// 雷球発射の移動速度
#define FLAME_MOVE_SPEED 100.0f
// 雷球の色
#define FLAME_COLOR CColor(1.0f, 1.0f, 0.0f)

// コンストラクタ
CLightningBallEffect::CLightningBallEffect(CObjectBase* owner, const CMatrix* attach,
	const CVector& offsetPos, const CMatrix& offsetRot)
	: mpOwner(owner)
	, mpAttachMtx(attach)
	, mThrowOffsetPos(offsetPos)
	, mThrowOffsetRot(offsetRot)
	, mElapsedTime(0.0f)
	, mIsThrowing(false)
{
	spInstance = this;
}

// デストラクタ
CLightningBallEffect::~CLightningBallEffect()
{
	spInstance = nullptr;
}

// 雷球発射を開始
void CLightningBallEffect::Start()
{
	mIsThrowing = true;
	mElapsedTime = 0.0f;
}

// 雷球発射を停止
void CLightningBallEffect::Stop()
{
	mIsThrowing = false;
}

// 雷球をしているかどうか
bool CLightningBallEffect::IsThrowing() const
{
	return mIsThrowing;
}

// 雷球発射時のオフセット位置を設定
void CLightningBallEffect::SetThrowOffsetPos(const CVector& pos)
{
	mThrowOffsetPos = pos;
}

// 雷球発射時のオフセット回転値を設定
void CLightningBallEffect::SetThrowOffsetRot(const CMatrix& rot)
{
	mThrowOffsetRot = rot;
}

// 雷球発射の位置を取得
CVector CLightningBallEffect::GetThrowPos() const
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

// 雷球発射の方向を取得
CVector CLightningBallEffect::GetThrowDir() const
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

// 雷球を作成
void CLightningBallEffect::CreateLightningBall()
{
	// 雷球を作成
	CLightningBall* lightningBall = new CLightningBall(ETag::eLightningBall);

	// 位置を取得
	CVector pos = GetThrowPos();// + CVector(0.0f, 10.0f, 0.0f);
	// 方向を取得
	CVector dir = GetThrowDir();// + CVector(0.0f, -1.0f, 0.0f);
	// 方向をランダムでブラす
	dir.X(dir.X() + Math::Rand(-FLAME_DIR_RAND, FLAME_DIR_RAND));
	dir.Y(dir.Y() + Math::Rand(-FLAME_DIR_RAND, FLAME_DIR_RAND));
	dir.Z(dir.Z() + Math::Rand(-FLAME_DIR_RAND, FLAME_DIR_RAND));
	dir.Normalize();
	// 位置、方向、移動速度を設定
	lightningBall->Setup(pos, dir, FLAME_MOVE_SPEED);

	// 雷球のカラーを設定
	lightningBall->SetColor(FLAME_COLOR);
	// 加算ブレンドにして、雷球が発光しているように見せる
	lightningBall->SetBlendType(EBlend::eAdd);

	// 作成した雷のエフェクトをリストに追加
	//mLightningBall.push_back(lightningBall);
}

// 更新
void CLightningBallEffect::Update()
{
	// 雷球を発射していたら
	if (mIsThrowing)
	{
		// 経過時間に応じて、雷球を作成
		if (mElapsedTime >= THROW_INTERVAL)
		{
			CreateLightningBall();
			mElapsedTime -= THROW_INTERVAL;
		}
		mElapsedTime += Time::DeltaTime();
	}
}