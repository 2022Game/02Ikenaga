#include "CImpactEffect.h"
#include "CImpact.h"
#include "Maths.h"

// 衝撃の間隔時間
#define THROW_INTERVAL 0.4f
// 衝撃の方向のブレ幅
#define FLAME_DIR_RAND 0.02f
// 衝撃の移動速度
#define FLAME_MOVE_SPEED 70.0f
// 衝撃の色
//#define FLAME_COLOR CColor(1.0f, 1.0f, 0.0f)

// コンストラクタ
CImpactEffect::CImpactEffect(CObjectBase* owner, const CMatrix* attach,
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
CImpactEffect::~CImpactEffect()
{
}

// 衝撃を開始
void CImpactEffect::Start()
{
	mIsThrowing = true;
	mElapsedTime = 0.0f;
}

// 衝撃を停止
void CImpactEffect::Stop()
{
	mIsThrowing = false;
}

// 衝撃をしているかどうか
bool CImpactEffect::IsThrowing() const
{
	return mIsThrowing;
}

// 衝撃時のオフセット位置を設定
void CImpactEffect::SetThrowOffsetPos(const CVector& pos)
{
	mThrowOffsetPos = pos;
}

// 衝撃時のオフセット回転値を設定
void CImpactEffect::SetThrowOffsetRot(const CMatrix& rot)
{
	mThrowOffsetRot = rot;
}

// 衝撃の位置を取得
CVector CImpactEffect::GetThrowPos() const
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

// 衝撃の方向を取得
CVector CImpactEffect::GetThrowDir() const
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

// 衝撃を作成
void CImpactEffect::CreateImpact()
{
	// 衝撃を作成
	CImpact* impact = new CImpact(ETag::eImpact);

	// 位置を取得
	CVector pos = GetThrowPos();// + CVector(0.0f, 10.0f, 0.0f);
	// 方向を取得
	CVector dir = GetThrowDir();// + CVector(0.0f, -1.0f, 0.0f);
	// 方向をランダムでブラす
	//dir.X(dir.X() + Math::Rand(-FLAME_DIR_RAND, FLAME_DIR_RAND));
	//dir.Y(dir.Y() + Math::Rand(-FLAME_DIR_RAND, FLAME_DIR_RAND));
	//dir.Z(dir.Z() + Math::Rand(-FLAME_DIR_RAND, FLAME_DIR_RAND));
	//dir.Normalize();
	// 位置、方向、移動速度を設定
	impact->Setup(pos, dir, FLAME_MOVE_SPEED);

	// 衝撃のカラーを設定
	//impact->SetColor(FLAME_COLOR);
	// 加算ブレンドにして、衝撃が発光しているように見せる
	impact->SetBlendType(EBlend::eAdd);

	// 作成した衝撃のエフェクトをリストに追加
	mImpact.push_back(impact);
}

// 更新
void CImpactEffect::Update()
{
	// 衝撃を発射していたら
	if (mIsThrowing)
	{
		// 経過時間に応じて、衝撃を作成
		if (mElapsedTime >= THROW_INTERVAL)
		{
			CreateImpact();
			mElapsedTime -= THROW_INTERVAL;
		}
		mElapsedTime += Time::DeltaTime();
	}
}