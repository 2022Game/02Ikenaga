#include "CBuffSkill.h"
#include "CImage.h"

// コンストラク
CBuffSkill::CBuffSkill(float x, float y)
{
	mpBuffSkill = new CImage("BuffSkill");
	mpBuffSkill->SetPos(x, y);
	mpBuffSkill->SetSize(100.0f, 100.0f);
	mpBuffSkill->SetColor(1.0f,1.0f,1.0f,0.9f);
}

// デストラクタ
CBuffSkill::~CBuffSkill()
{
}