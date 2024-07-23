#pragma once
#include "CUIBase.h"

class CImage;

// 歯車
class CGear : public CUIBase
{
public:
	// コンストラク
	CGear(float x, float y);

	// デストラクタ
	~CGear();

private:
	CImage* mpGear;
};