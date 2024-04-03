#include "CSlash.h"
#include "CSlashing.h"
#include "CInput.h"
#include "CPlayer.h"
#include "Maths.h"

CSlash* CSlash::spInstance = nullptr;

// スラッシュの発射間隔時間
#define THROW_INTERVAL 0.8f
// スラッシュの発射方向のブレ幅
#define FLAME_DIR_RAND 0.02f
// スラッシュの移動速度
#define FLAME_MOVE_SPEED 75.0f
// スラッシュの色
#define FLAME_COLOR CColor(0.0f, 1.0f, 0.9f)

// コンストラクタ
CSlash::CSlash(CObjectBase* owner, const CMatrix* attach,
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
CSlash::~CSlash()
{
	spInstance = nullptr;
}

// スラッシュを発射開始
void CSlash::Start()
{
	mIsThrowing = true;
	mElapsedTime = 0.0f;
}

// スラッシュを発射停止
void CSlash::Stop()
{
	mIsThrowing = false;
}

// スラッシュを発射しているかどうか
bool CSlash::IsThrowing() const
{
	return mIsThrowing;
}

// 発射時のオフセット位置を設定
void CSlash::SetThrowOffsetPos(const CVector& pos)
{
	mThrowOffsetPos = pos;
}

// 発射時のオフセット回転値を設定
void CSlash::SetThrowOffsetRot(const CMatrix& rot)
{
	mThrowOffsetRot = rot;
}

// スラッシュの発射位置を取得
CVector CSlash::GetThrowPos() const
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

// スラッシュの発射方向を取得
CVector CSlash::GetThrowDir() const
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

// スラッシュを作成
void CSlash::CreateSlash()
{
	// スラッシュを作成
	CSlashing* slash = new CSlashing(ETag::eSlash);

	// 発射位置を取得
	CVector pos = GetThrowPos();// + CVector(0.0f, 10.0f, 0.0f);
	// 発射方向を取得
	CVector dir = GetThrowDir();// + CVector(0.0f, -1.0f, 0.0f);
	// 発射方向をランダムでブラす
	//dir.X(dir.X() + Math::Rand(-FLAME_DIR_RAND, FLAME_DIR_RAND));
	//dir.Y(dir.Y() + Math::Rand(-FLAME_DIR_RAND, FLAME_DIR_RAND));
	//dir.Z(dir.Z() + Math::Rand(-FLAME_DIR_RAND, FLAME_DIR_RAND));
	dir.Normalize();
	// 発射位置、方向、移動速度を設定
	slash->Setup(pos, dir, FLAME_MOVE_SPEED);

	// プレイヤーのいる方向へ向く
	//CVector dir2 = player->Position() - slash->Position();
	//dir2.Y(0.0f);
	//dir2.Normalize();
	//slash->Rotation(CQuaternion::LookRotation(dir2));
	
	//CPlayer* player = CPlayer::Instance();
	//slash->Rotation(75.0f, 0.0f, 0.0f);
		//slash->Rotation(75.0f, 180.0f, 0.0f);
		//slash->Rotation(75.0f, 270.0f, 0.0f);
	slash->Rotation(75.0f, -270.0f, 0.0f);
	// スラッシュのカラーを設定
	slash->SetColor(FLAME_COLOR);
	// 加算ブレンドにして、スラッシュが発光しているように見せる
	slash->SetBlendType(EBlend::eAdd);
}

// 更新
void CSlash::Update()
{
	// スラッシュを発射していたら
	if (mIsThrowing)
	{
		// 経過時間に応じて、スラッシュを作成
		if (mElapsedTime >= THROW_INTERVAL)
		{
			CreateSlash();
			mElapsedTime -= THROW_INTERVAL;
		}
		mElapsedTime += Time::DeltaTime();
	}
}