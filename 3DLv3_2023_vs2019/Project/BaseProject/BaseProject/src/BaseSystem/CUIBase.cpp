#include "CUIBase.h"

CUIBase::CUIBase()
	: CTask(ETaskPriority::eUI)
	, mPosition(0.0f, 0.0f)
	, mSize(100.0f, 100.0f)
	, mColor(1.0f, 1.0f, 1.0f, 1.0f)
{
}

CUIBase::~CUIBase()
{
}

//表示座標を設定（CVector2版）
void CUIBase::SetPos(const CVector2& pos)
{
	mPosition = pos;
}

//表示座標を設定
void CUIBase::SetPos(const float& x, const float& y)
{
	SetPos(CVector2(x, y));
}

//表示座標を取得
const CVector2& CUIBase::GetPos() const
{
	return mPosition;
}

//表示サイズを設定（CVector2版）
void CUIBase::SetSize(const CVector2& size)
{
	mSize = size;
}

//表示サイズを設定
void CUIBase::SetSize(const float& x, const float& y)
{
	SetSize(CVector2(x, y));
}

//表示サイズを取得
const CVector2& CUIBase::GetSize() const
{
	return mSize;
}

//表示カラーを設定（CColor版）
void CUIBase::SetColor(const CColor& color)
{
	mColor = color;
}

//表示カラーを設定（アルファ値有り版）
void CUIBase::SetColor(const float& r, const float& g, const float& b, const float& a)
{
	mColor.Set(r, g, b, a);
}

//表示カラーを設定
void CUIBase::SetColor(const float& r, const float& g, const float& b)
{
	mColor.Set(r, g, b);
}

//表示カラーを取得
const CColor& CUIBase::GetColor() const
{
	return mColor;
}

//アルファ値を設定
void CUIBase::SetAlpha(const float& alpha)
{
	mColor.A(alpha);
}

//アルファ値を取得
const float& CUIBase::GetAlpha() const
{
	return mColor.A();
}
