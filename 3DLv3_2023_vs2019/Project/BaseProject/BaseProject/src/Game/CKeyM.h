#pragma once
#include "CUIBase.h"

class CImage;

// Mキー
class CKeyM : public CUIBase
{
public:
	// コンストラク
	CKeyM(float x, float y);

	// デストラクタ
	~CKeyM();

private:
	CImage* mpKeyM;
};