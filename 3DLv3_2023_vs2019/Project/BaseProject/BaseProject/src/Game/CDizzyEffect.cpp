#include "CDizzyEffect.h"
#include "CDizzy.h"
#include "Maths.h"
#include "CCharaBase.h"

// コンストラクタ
CDizzyEffect::CDizzyEffect(CObjectBase* owner, const CMatrix* attach,
	const CVector& offsetPos)
	: mpOwner(owner)
	, mpAttachMtx(attach)
	, mOffsetPos(offsetPos)
	, mIsDizzy(false)
	, mStep(0)
{
}

// デストラクタ
CDizzyEffect::~CDizzyEffect()
{
}

// 開始
void CDizzyEffect::Start()
{
	mIsDizzy = true;
	mStep = 0;
}

// 停止
void CDizzyEffect::Stop()
{
	mIsDizzy = false;
}

// めまいをしているかどうか
bool CDizzyEffect::IsDizzy() const
{
	return mIsDizzy;
}

// オフセット位置を設定
void CDizzyEffect::SetDizzyOffsetPos(const CVector& pos)
{
	mOffsetPos = pos;
}

// 位置を取得
CVector CDizzyEffect::GetPos() const
{
	// アタッチする行列が設定されている場合は、行列の座標を返す
	if (mpAttachMtx != nullptr)
	{
		CVector pos = mpAttachMtx->Position();
		pos += mpAttachMtx->VectorX().Normalized() * mOffsetPos.X()
			+ mpAttachMtx->VectorY().Normalized() * mOffsetPos.Y()
			+ mpAttachMtx->VectorZ().Normalized() * mOffsetPos.Z();
		return pos;
	}
	// 持ち主が設定されている場合は、持ち主の座標を返す
	else if (mpOwner != nullptr)
	{
		return mpOwner->Position() + (mpOwner->Rotation() * mOffsetPos);
	}

	return CVector::zero;
}

// めまいを作成
void CDizzyEffect::CreateDizzy()
{
	// めまいを作成
	CDizzy* dizzy = new CDizzy();
	CCharaBase* owner = dynamic_cast<CCharaBase*>(mpOwner);
	if (owner != nullptr)
	{
		dizzy->SetOwner(owner);
	}

	// 位置を取得
	CVector pos = GetPos();// + CVector(0.0f, 10.0f, 0.0f);
	dizzy->Setup(pos);
	dizzy->SetSize(CVector2(20, 20));

	mDizzy.push_back(dizzy);
}

// 更新
void CDizzyEffect::Update()
{
	if (mIsDizzy)
	{
		switch (mStep)
		{
		case 0:
			CreateDizzy();
			mStep++;
			break;
		case 1:
			break;
		}
	}
}