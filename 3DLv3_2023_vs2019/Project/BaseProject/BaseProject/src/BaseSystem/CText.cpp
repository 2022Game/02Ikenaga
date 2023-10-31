#include "CText.h"
#include <glut.h>
#include <locale.h>

CText::CText(CFont* font, int fontSize, const CVector2& pos,
	const CVector2& size, const CColor& color)
	: mpFont(font)
	, mText(L"")
	, mFontSize(fontSize)
{
	mPosition = pos;
	mSize = size;
	mColor = color;

	if (mpFont == nullptr)
	{
		mpFont = new CFont();
		mpFont->SetFontSize(mFontSize);
		mpFont->SetLineLength(mSize.X());
	}
}

CText::~CText()
{
}

void CText::SetFontSize(int fontSize)
{
	mFontSize = fontSize;

	if (mpFont == nullptr) return;
	mpFont->SetFontSize(fontSize);
}

void CText::SetSize(const CVector2& size)
{
	mSize = size;
	mpFont->SetLineLength(size.X());
}

void CText::SetTextAlign(ETextAlignH alignH, ETextAlignV alignV)
{
	SetTextAlignH(alignH);
	SetTextAlignV(alignV);
}

void CText::SetTextAlignH(ETextAlignH align)
{
	mTextAlignH = align;
	switch (mTextAlignH)
	{
		case ETextAlignH::eLeft:
			mpFont->SetAlignment(FTGL::ALIGN_LEFT);
			break;
		case ETextAlignH::eCenter:
			mpFont->SetAlignment(FTGL::ALIGN_CENTER);
			break;
		case ETextAlignH::eRight:
			mpFont->SetAlignment(FTGL::ALIGN_RIGHT);
			break;
	}
}

void CText::SetTextAlignV(ETextAlignV align)
{
	mTextAlignV = align;
	switch (mTextAlignV)
	{
		case ETextAlignV::eTop:
			break;
		case ETextAlignV::eMiddle:
			break;
		case ETextAlignV::eBottom:
			break;
	}
}

void CText::SetText(const char* format, ...)
{
	char buf[256];
	va_list ap;
	va_start(ap, format);
	vsprintf_s(buf, format, ap);
	va_end(ap);

	wchar_t wbuf[256];
	setlocale(LC_CTYPE, "jpn");
	size_t len = mbstowcs(wbuf, buf, 256);
	if (len >= 0)
	{
		mText = wbuf;
	}
	else
	{
		mText = L"";
	}
}

void CText::SetText(const wchar_t* format, ...)
{
	wchar_t wbuf[256];
	va_list ap;
	va_start(ap, format);
	swprintf_s(wbuf, format, ap);
	va_end(ap);

	mText = wbuf;
}

void CText::Render()
{
	glPushMatrix();

	float sx = mPosition.X();
	float ex = mPosition.X() + mSize.X();
	float sy = mPosition.Y();
	float ey = mPosition.Y() + mSize.Y();

	//glBegin(GL_QUADS);
	//glVertex2f(sx, sy);
	//glVertex2f(sx, ey);
	//glVertex2f(ex, ey);
	//glVertex2f(ex, sy);
	//glEnd();

	//F‚ÌÝ’è
	glColor4f(mColor.R(), mColor.G(), mColor.B(), mColor.A());

	glRasterPos2f(mPosition.X(), mPosition.Y() + mFontSize);

	mpFont->Render(mText.c_str());

	glPopMatrix();
}