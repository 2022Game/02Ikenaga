#include "CRoarEffect.h"
#include "CRoar.h"
#include "CInput.h"
#include "Maths.h"

CRoarEffect* CRoarEffect::spInstance = nullptr;

// 雄叫びの間隔時間
#define THROW_INTERVAL 0.6f
// 雄叫びの方向のブレ幅
#define FLAME_DIR_RAND 0.02f
// 雄叫びの移動速度
#define FLAME_MOVE_SPEED 25.0f
// 雄叫びの色
#define FLAME_COLOR CColor(1.0f, 1.0f, 1.0f)

// コンストラクタ
CRoarEffect::CRoarEffect(CObjectBase* owner, const CMatrix* attach,
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
CRoarEffect::~CRoarEffect()
{
	spInstance = nullptr;
}

// 雄叫びを開始
void CRoarEffect::Start()
{
	mIsThrowing = true;
	mElapsedTime = 0.0f;
}

// 雄叫びを停止
void CRoarEffect::Stop()
{
	mIsThrowing = false;
}

// 雄叫びをしているかどうか
bool CRoarEffect::IsThrowing() const
{
	return mIsThrowing;
}

// 雄叫び時のオフセット位置を設定
void CRoarEffect::SetThrowOffsetPos(const CVector& pos)
{
	mThrowOffsetPos = pos;
}

// 雄叫び時のオフセット回転値を設定
void CRoarEffect::SetThrowOffsetRot(const CMatrix& rot)
{
	mThrowOffsetRot = rot;
}

// 雄叫びの位置を取得
CVector CRoarEffect::GetThrowPos() const
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

// 雄叫びの方向を取得
CVector CRoarEffect::GetThrowDir() const
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

// 雄叫びを作成
void CRoarEffect::CreateSlash()
{
	// 雄叫びを作成
	CRoar* roar = new CRoar(ETag::eNone);

	// 位置を取得
	CVector pos = GetThrowPos();// + CVector(0.0f, 10.0f, 0.0f);
	// 方向を取得
	CVector dir = GetThrowDir();// + CVector(0.0f, -1.0f, 0.0f);
	// 方向をランダムでブラす
	//dir.X(dir.X() + Math::Rand(-FLAME_DIR_RAND, FLAME_DIR_RAND));
	//dir.Y(dir.Y() + Math::Rand(-FLAME_DIR_RAND, FLAME_DIR_RAND));
	//dir.Z(dir.Z() + Math::Rand(-FLAME_DIR_RAND, FLAME_DIR_RAND));
	dir.Normalize();
	// 位置、方向、移動速度を設定
	roar->Setup(pos, dir, FLAME_MOVE_SPEED);

	//slash->Rotation(40.0f, 0.0f, 0.0f);
	// 雄叫びのカラーを設定
	roar->SetColor(FLAME_COLOR);
	// 加算ブレンドにして、雄叫びが発光しているように見せる
	roar->SetBlendType(EBlend::eAdd);
}

// 更新
void CRoarEffect::Update()
{
	// 雄叫びを発射していたら
	if (mIsThrowing)
	{
		// 経過時間に応じて、雄叫びを作成
		if (mElapsedTime >= THROW_INTERVAL)
		{
			CreateSlash();
			mElapsedTime -= THROW_INTERVAL;
		}
		mElapsedTime += Time::DeltaTime();
	}
}