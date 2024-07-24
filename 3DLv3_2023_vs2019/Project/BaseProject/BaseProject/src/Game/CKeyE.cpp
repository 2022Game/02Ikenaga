#include "CKeyE.h"
#include "CImage.h"

// コンストラク
CKeyE::CKeyE(float x, float y)
{
	mpKeyE = new CImage("KeyE");
	mpKeyE->SetPos(x, y);
	mpKeyE->SetSize(50.0f, 50.0f);
}

// デストラクタ
CKeyE::~CKeyE()
{
}