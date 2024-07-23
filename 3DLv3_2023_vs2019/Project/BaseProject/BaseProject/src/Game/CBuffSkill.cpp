#include "CBuffSkill.h"
#include "CImage.h"

// コンストラク
CBuffSkill::CBuffSkill(float x, float y)
{
	mpBuffSkill = new CImage("BuffSkill");
	mpBuffSkill->SetPos(x, y);
	mpBuffSkill->SetSize(100.0f, 100.0f);
}

// デストラクタ
CBuffSkill::~CBuffSkill()
{
}