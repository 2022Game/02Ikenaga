#pragma once
#include "CUIBase.h"

class CImage;

// バフスキル
class CBuffSkill : public CUIBase
{
public:
	// コンストラク
	CBuffSkill(float x, float y);

	// デストラクタ
	~CBuffSkill();

private:
	CImage* mpBuffSkill;
};