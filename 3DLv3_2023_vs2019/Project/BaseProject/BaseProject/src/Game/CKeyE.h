#pragma once
#include "CUIBase.h"

class CImage;

// Eキー
class CKeyE : public CUIBase
{
public:
	// コンストラク
	CKeyE(float x, float y);

	// デストラクタ
	~CKeyE();

private:
	CImage* mpKeyE;
};