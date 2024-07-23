#include "CMenu.h"
#include "CImage.h"

// コンストラク
CMenu::CMenu(float x, float y)
{
	mpMenu = new CImage("Menu");
	mpMenu->SetPos(x, y);
	mpMenu->SetSize(30.0f, 30.0f);
}

// デストラクタ
CMenu::~CMenu()
{
}
