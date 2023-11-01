#pragma once
#include "CTask.h"
#include "CVector.h"
#include "CColor.h"

class CUIBase : public CTask
{
public:
	CUIBase(ETaskPriority prio = ETaskPriority::eUI, bool dontDelete = false);
	virtual ~CUIBase();

	//表示座標を設定（CVector2版）
	virtual void SetPos(const CVector2& pos);
	//表示座標を設定
	virtual void SetPos(const float& x, const float& y);
	//表示座標を取得
	const CVector2& GetPos() const;

	//表示サイズを設定（CVector2版）
	virtual void SetSize(const CVector2& size);
	//表示サイズを設定
	virtual void SetSize(const float& x, const float& y);
	//表示サイズを取得
	const CVector2& GetSize() const;

	//表示カラーを設定（CColor版）
	virtual void SetColor(const CColor& color);
	//表示カラーを設定（アルファ値有り版）
	virtual void SetColor(const float& r, const float& g, const float& b, const float& a);
	//表示カラーを設定
	virtual void SetColor(const float& r, const float& g, const float& b);
	//表示カラーを取得
	const CColor& GetColor() const;

	//アルファ値を設定
	virtual void SetAlpha(const float& alpha);
	//アルファ値を取得
	const float& GetAlpha() const;

protected:
	CVector2 mPosition;	//表示座標
	CVector2 mSize;		//表示サイズ
	CColor mColor;		//UIのカラー
private:
};