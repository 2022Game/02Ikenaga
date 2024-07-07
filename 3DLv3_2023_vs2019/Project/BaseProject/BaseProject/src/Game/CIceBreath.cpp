#include "CIceBreath.h"
#include "CBreath.h"
#include "Maths.h"
#include "CCharaBase.h"

// アイスブレスの発射間隔時間
#define THROW_INTERVAL 0.01f
// アイスブレスの発射方向のブレ幅
#define BREATH_DIR_RAND 0.01f
// アイスブレスの移動速度
#define BREATH_MOVE_SPEED 50.0f
// アイスブレスの色
#define BREATH_COLOR CColor(0.0f, 1.0f, 1.0f)

// コンストラクタ
CIceBreath::CIceBreath(CObjectBase* owner, const CMatrix* attach,
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
CIceBreath::~CIceBreath()
{
	for (CBreath* breath : mBreath)
	{
		breath->Kill();
	}
}

// 発射開始
void CIceBreath::Start()
{
	mIsThrowing = true;
	mElapsedTime = 0.0f;
}

// 発射停止
void CIceBreath::Stop()
{
	mIsThrowing = false;
}

// アイスブレスを発射しているかどうか
bool CIceBreath::IsThrowing() const
{
	return mIsThrowing;
}

// 発射時のオフセット位置を設定
void CIceBreath::SetThrowOffsetPos(const CVector& pos)
{
	mThrowOffsetPos = pos;
}

// 発射時のオフセット回転値を設定
void CIceBreath::SetThrowOffsetRot(const CMatrix& rot)
{
	mThrowOffsetRot = rot;
}

// アイスブレスの発射位置を取得
CVector CIceBreath::GetThrowPos() const
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

// アイスブレスの発射方向を取得
CVector CIceBreath::GetThrowDir() const
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

	return mThrowOffsetRot * CVector::forward;
}

// ブレスのエフェクトを作成
void CIceBreath::CreateBreath()
{
	CBreath* breath = new CBreath(ETag::eFlame);
	CCharaBase* owner = dynamic_cast<CCharaBase*>(mpOwner);
	if (owner != nullptr)
	{
		breath->SetOwner(owner);
	}

	// 発射位置を取得
	CVector pos = GetThrowPos();// + CVector(0.0f, 10.0f, 0.0f);
	// 発射方向を取得
	CVector dir = GetThrowDir();// + CVector(0.0f, -1.0f, 0.0f);
	// 発射方向をランダムでブラす
	dir.X(dir.X() + Math::Rand(-BREATH_DIR_RAND, BREATH_DIR_RAND));
	dir.Y(dir.Y() + Math::Rand(0.0f, 0.05f));
	dir.Z(dir.Z() + Math::Rand(-BREATH_DIR_RAND, BREATH_DIR_RAND));
	dir.Normalize();
	// 発射位置、方向、移動速度を設定
	breath->Setup(pos, dir, BREATH_MOVE_SPEED);

	// ブレスのカラーを設定
	breath->SetColor(BREATH_COLOR);
	// 加算ブレンドにして、ブレスが発光しているように見せる
	breath->SetBlendType(EBlend::eAdd);

	// 作成したブレスのエフェクトをリストに追加
	mBreath.push_back(breath);
}

// 更新
void CIceBreath::Update()
{
	// ブレスを発射していたら
	if (mIsThrowing)
	{
		// 経過時間に応じて、ブレスのエフェクトを作成
		if (mElapsedTime >= THROW_INTERVAL)
		{
			CreateBreath();
			mElapsedTime -= THROW_INTERVAL;
		}
		mElapsedTime += Time::DeltaTime();
	}

	// 生成済みのブレスのエフェクトの削除処理
	auto itr = mBreath.begin();
	auto end = mBreath.end();
	while (itr != end)
	{
		CBreath* breath = *itr;
		// 削除フラグが立っていたら、削除
		if (breath->IsDeath())
		{
			itr = mBreath.erase(itr);
			breath->Kill();
		}
		else
		{
			itr++;
		}
	}
}