#pragma once
#include "CUIBase.h"

class CImage;

// Qキー
class CKeyQ : public CUIBase
{
public:
	// コンストラク
	CKeyQ(float x, float y);

	// デストラクタ
	~CKeyQ();

private:
	CImage* mpKeyQ;
};