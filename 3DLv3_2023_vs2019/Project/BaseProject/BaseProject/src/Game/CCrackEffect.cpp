#include "CCrackEffect.h"
#include "CCrack.h"
#include "Maths.h"

CCrackEffect* CCrackEffect::spInstance = nullptr;

// ひび割れの間隔時間
#define THROW_INTERVAL 0.4f
// ひび割れの方向のブレ幅
#define FLAME_DIR_RAND 0.02f
// ひび割れの移動速度
#define FLAME_MOVE_SPEED 25.0f
// ひび割れの色
#define FLAME_COLOR CColor(1.0f, 1.0f, 1.0f)

// コンストラクタ
CCrackEffect::CCrackEffect(CObjectBase* owner, const CMatrix* attach,
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
CCrackEffect::~CCrackEffect()
{
	spInstance = nullptr;
}

// ひび割れを開始
void CCrackEffect::Start()
{
	mIsThrowing = true;
	mElapsedTime = 0.0f;
}

// ひび割れを停止
void CCrackEffect::Stop()
{
	mIsThrowing = false;
}

// ひび割れをしているかどうか
bool CCrackEffect::IsThrowing() const
{
	return mIsThrowing;
}

// ひび割れ時のオフセット位置を設定
void CCrackEffect::SetThrowOffsetPos(const CVector& pos)
{
	mThrowOffsetPos = pos;
}

// ひび割れ時のオフセット回転値を設定
void CCrackEffect::SetThrowOffsetRot(const CMatrix& rot)
{
	mThrowOffsetRot = rot;
}

// ひび割れの位置を取得
CVector CCrackEffect::GetThrowPos() const
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

// ひび割れの方向を取得
CVector CCrackEffect::GetThrowDir() const
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

// ひび割れを作成
void CCrackEffect::CreateSlash()
{
	// ひび割れを作成
	CCrack* crack = new CCrack(ETag::eNone);

	// 位置を取得
	CVector pos = GetThrowPos();// + CVector(0.0f, 10.0f, 0.0f);
	// 方向を取得
	CVector dir = GetThrowDir();// + CVector(0.0f, -1.0f, 0.0f);
	dir.Normalize();
	// 位置、方向、移動速度を設定
	crack->Setup(pos, dir, FLAME_MOVE_SPEED);
	// ひび割れのカラーを設定
	crack->SetColor(FLAME_COLOR);
	// 加算ブレンドにして、ひび割れが発光しているように見せる
	crack->SetBlendType(EBlend::eAdd);
}

// 更新
void CCrackEffect::Update()
{
	// ひび割れを発射していたら
	if (mIsThrowing)
	{
		// 経過時間に応じて、ひび割れを作成
		if (mElapsedTime >= THROW_INTERVAL)
		{
			CreateSlash();
			mElapsedTime -= THROW_INTERVAL;
		}
		mElapsedTime += Time::DeltaTime();
	}
}