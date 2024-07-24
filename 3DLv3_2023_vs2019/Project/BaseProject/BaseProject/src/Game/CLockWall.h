#pragma once
#include "CUIBase.h"

class CImage;

// ロック中の壁
class CLockWall : public CUIBase
{
public:
	// コンストラク
	CLockWall(float x, float y);

	// デストラクタ
	~CLockWall();

	// 更新
	void Update();

private:
	CImage* mpLockWall;
	int mLevel;
};