#pragma once
#include "CUIBase.h"
#include "CFont.h"
#include "CVector.h"
#include "CColor.h"

enum ETextAlignH
{
	eLeft,	// ç∂ëµÇ¶
	eCenter,// íÜâõëµÇ¶
	eRight,	// âEëµÇ¶
};

enum ETextAlignV
{
	eTop,	// è„ëµÇ¶
	eMiddle,// è„â∫íÜâõëµÇ¶
	eBottom,// â∫ëµÇ¶
};

class CText : public CUIBase
{
public:
	CText(CFont* font = nullptr, int fontSize = 24,
		const CVector2& pos = CVector2::zero,
		const CVector2& size = CVector2(400.0f, 300.0f),
		const CColor& color = CColor::black);
	~CText();

	void SetFontSize(int fontSize);

	void SetSize(const CVector2& size);

	void SetTextAlign(ETextAlignH alignH, ETextAlignV alignV);
	void SetTextAlignH(ETextAlignH align);
	void SetTextAlignV(ETextAlignV align);

	void SetText(const char* format, ...);
	void SetText(const wchar_t* format, ...);

	void Render();

private:
	CFont* mpFont;
	std::wstring mText;
	int mFontSize;
	ETextAlignH mTextAlignH;
	ETextAlignV mTextAlignV;
};