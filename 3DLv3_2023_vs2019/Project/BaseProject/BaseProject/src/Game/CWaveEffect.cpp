#include "CWaveEffect.h"
#include "CWave.h"
#include "Maths.h"

CWaveEffect* CWaveEffect::spInstance = nullptr;

// 波動の間隔時間
#define THROW_INTERVAL 0.2f
// 波動の方向のブレ幅
#define FLAME_DIR_RAND 0.02f
// 波動の移動速度
#define FLAME_MOVE_SPEED 25.0f
// 波動の色
#define FLAME_COLOR CColor(0.0f, 0.5f, 1.0f)

// コンストラクタ
CWaveEffect::CWaveEffect(CObjectBase* owner, const CMatrix* attach,
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
CWaveEffect::~CWaveEffect()
{
	spInstance = nullptr;
}

// 波動を開始
void CWaveEffect::Start()
{
	mIsThrowing = true;
	mElapsedTime = 0.0f;
}

// 波動を停止
void CWaveEffect::Stop()
{
	mIsThrowing = false;
}

// 波動をしているかどうか
bool CWaveEffect::IsThrowing() const
{
	return mIsThrowing;
}

// 波動時のオフセット位置を設定
void CWaveEffect::SetThrowOffsetPos(const CVector& pos)
{
	mThrowOffsetPos = pos;
}

// 波動時のオフセット回転値を設定
void CWaveEffect::SetThrowOffsetRot(const CMatrix& rot)
{
	mThrowOffsetRot = rot;
}

// 波動の位置を取得
CVector CWaveEffect::GetThrowPos() const
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

// 波動の方向を取得
CVector CWaveEffect::GetThrowDir() const
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

// 波動を作成
void CWaveEffect::CreateWave()
{
	// 雄叫びを作成
	CWave* wave = new CWave(ETag::eWave);

	// 位置を取得
	CVector pos = GetThrowPos();// + CVector(0.0f, 10.0f, 0.0f);
	// 方向を取得
	CVector dir = GetThrowDir();// + CVector(0.0f, -1.0f, 0.0f);
	// 方向をランダムでブラす
	dir.X(dir.X() + Math::Rand(-FLAME_DIR_RAND, FLAME_DIR_RAND));
	dir.Y(dir.Y() + Math::Rand(-FLAME_DIR_RAND, FLAME_DIR_RAND));
	dir.Z(dir.Z() + Math::Rand(-FLAME_DIR_RAND, FLAME_DIR_RAND));
	//dir.Normalize();
	// 位置、方向、移動速度を設定
	wave->Setup(pos, dir, FLAME_MOVE_SPEED);

	// 波動のカラーを設定
	wave->SetColor(FLAME_COLOR);
	// 加算ブレンドにして、波動が発光しているように見せる
	wave->SetBlendType(EBlend::eAdd);
}

// 更新
void CWaveEffect::Update()
{
	// 波動を発射していたら
	if (mIsThrowing)
	{
		// 経過時間に応じて、波動を作成
		if (mElapsedTime >= THROW_INTERVAL)
		{
			CreateWave();
			mElapsedTime -= THROW_INTERVAL;
		}
		mElapsedTime += Time::DeltaTime();
	}
}