#include "CKeyQ.h"
#include "CImage.h"

// コンストラク
CKeyQ::CKeyQ(float x, float y)
{
	mpKeyQ = new CImage("KeyQ");
	mpKeyQ->SetPos(x, y);
	mpKeyQ->SetSize(50.0f, 50.0f);
}

// デストラクタ
CKeyQ::~CKeyQ()
{
}