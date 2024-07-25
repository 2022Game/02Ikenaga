#pragma once
#include "CUIBase.h"

class CImage;

// ロック中の壁2
class CLockWall2 : public CUIBase
{
public:
	// コンストラク
	CLockWall2(float x, float y);

	// デストラクタ
	~CLockWall2();

	// 更新
	void Update();

private:
	CImage* mpLockWall;
	int mLevel;
};