#include "CKeyM.h"
#include "CImage.h"

// コンストラク
CKeyM::CKeyM(float x, float y)
{
	mpKeyM = new CImage("KeyM");
	mpKeyM->SetPos(x, y);
	mpKeyM->SetSize(50.0f, 50.0f);
}

// デストラクタ
CKeyM::~CKeyM()
{
}