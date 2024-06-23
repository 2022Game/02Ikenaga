#include "CDrainEffect.h"
#include "CDrain.h"
#include "Maths.h"
#include "CCharaBase.h"

// ドレインの発射の間隔時間
#define THROW_INTERVAL 0.8f
// ドレインの発射の方向のブレ幅
#define DIR_RAND 0.01f
// ドレインの色
#define COLOR CColor(1.0f, 0.0f, 0.5f)

// コンストラクタ
CDrainEffect::CDrainEffect(CObjectBase* owner, const CMatrix* attach,
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
CDrainEffect::~CDrainEffect()
{
}

// ドレインの発射を開始
void CDrainEffect::Start()
{
	mIsThrowing = true;
	mElapsedTime = 0.0f;
}

// ドレインの発射を停止
void CDrainEffect::Stop()
{
	mIsThrowing = false;
}

// ドレインをしているかどうか
bool CDrainEffect::IsThrowing() const
{
	return mIsThrowing;
}

// ドレインの発射時のオフセット位置を設定
void CDrainEffect::SetThrowOffsetPos(const CVector& pos)
{
	mThrowOffsetPos = pos;
}

// ドレインの発射時のオフセット回転値を設定
void CDrainEffect::SetThrowOffsetRot(const CMatrix& rot)
{
	mThrowOffsetRot = rot;
}

// ドレインの発射の位置を取得
CVector CDrainEffect::GetThrowPos() const
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

// ドレインの発射の方向を取得
CVector CDrainEffect::GetThrowDir() const
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

// ドレインを作成
void CDrainEffect::CreateDrain()
{
	// ドレインを作成
	CDrain* drain = new CDrain(ETag::eDrain);
	CCharaBase* owner = dynamic_cast<CCharaBase*>(mpOwner);
	if (owner != nullptr)
	{
		drain->SetOwner(owner);
	}

	// 位置を取得
	CVector pos = GetThrowPos();
	// 方向を取得
	CVector dir = GetThrowDir();
	// 位置、方向、移動速度を設定
	drain->Setup(pos, dir, 50.0f);

	// ドレインのカラーを設定
	drain->SetColor(COLOR);
	// 加算ブレンドにして、ドレインが発光しているように見せる
	drain->SetBlendType(EBlend::eAdd);
}

// 更新
void CDrainEffect::Update()
{
	// ドレインを発射していたら
	if (mIsThrowing)
	{
		// 経過時間に応じて、ドレインを作成
		if (mElapsedTime >= THROW_INTERVAL)
		{
			CreateDrain();
			mElapsedTime -= THROW_INTERVAL;
		}
		mElapsedTime += Time::DeltaTime();
	}
}