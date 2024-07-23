#pragma once
#include "CUIBase.h"

class CImage;

// メニューアイコン
class CMenu : public CUIBase
{
public:
	// コンストラク
	CMenu(float x, float y);

	// デストラクタ
	~CMenu();

private:
	CImage* mpMenu;
};