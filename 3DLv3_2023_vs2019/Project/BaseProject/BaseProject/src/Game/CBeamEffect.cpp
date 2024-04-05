#include "CBeamEffect.h"
#include "CBeam.h"
#include "Maths.h"

// ビームの間隔時間
#define THROW_INTERVAL 0.6f
// ビームの方向のブレ幅
#define FLAME_DIR_RAND 0.02f
// ビームの移動速度
#define FLAME_MOVE_SPEED 25.0f
// ビームの色
#define FLAME_COLOR CColor(1.0f, 1.0f, 0.0f)

// コンストラクタ
CBeamEffect::CBeamEffect(CObjectBase* owner, const CMatrix* attach,
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
CBeamEffect::~CBeamEffect()
{
	
}

// ビームを開始
void CBeamEffect::Start()
{
	mIsThrowing = true;
	mElapsedTime = 0.0f;
}

// ビームを停止
void CBeamEffect::Stop()
{
	mIsThrowing = false;
}

// ビームをしているかどうか
bool CBeamEffect::IsThrowing() const
{
	return mIsThrowing;
}

// ビーム時のオフセット位置を設定
void CBeamEffect::SetThrowOffsetPos(const CVector& pos)
{
	mThrowOffsetPos = pos;
}

// ビーム時のオフセット回転値を設定
void CBeamEffect::SetThrowOffsetRot(const CMatrix& rot)
{
	mThrowOffsetRot = rot;
}

// ビームの位置を取得
CVector CBeamEffect::GetThrowPos() const
{
	// アタッチする行列が設定されている場合は、行列の座標を返す
	if (mpAttachMtx != nullptr)
	{
		CVector pos = mpAttachMtx->Position();
		pos += mpAttachMtx->VectorX() * mThrowOffsetPos.X()
			+ mpAttachMtx->VectorY() * mThrowOffsetPos.Y()
			+ mpAttachMtx->VectorZ() * mThrowOffsetPos.Z();
		return pos;
	}
	// 持ち主が設定されている場合は、持ち主の座標を返す
	else if (mpOwner != nullptr)
	{
		return mpOwner->Position() + (mpOwner->Rotation() * mThrowOffsetPos);
	}

	return CVector::zero;
}

// ビームの方向を取得
CVector CBeamEffect::GetThrowDir() const
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

// ビームを作成
void CBeamEffect::CreateSlash()
{
	// ビームを作成
	CBeam* beam = new CBeam(ETag::eBeam);

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
	beam->Setup(pos, dir, FLAME_MOVE_SPEED);

	//slash->Rotation(40.0f, 0.0f, 0.0f);
	// ビームのカラーを設定
	beam->SetColor(FLAME_COLOR);
	// 加算ブレンドにして、ビームが発光しているように見せる
	beam->SetBlendType(EBlend::eAdd);
}

// 更新
void CBeamEffect::Update()
{
	// ビームを発射していたら
	if (mIsThrowing)
	{
		// 経過時間に応じて、ビームを作成
		if (mElapsedTime >= THROW_INTERVAL)
		{
			CreateSlash();
			mElapsedTime -= THROW_INTERVAL;
		}
		mElapsedTime += Time::DeltaTime();
	}
}