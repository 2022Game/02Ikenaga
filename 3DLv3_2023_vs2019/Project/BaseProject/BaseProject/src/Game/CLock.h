#pragma once
#include "CUIBase.h"

class CImage;

// ロック
class CLock : public CUIBase
{
public:
	// コンストラク
	CLock(float x, float y);

	// デストラクタ
	~CLock();

	// 更新
	void Update();

private:
	CImage* mpLock;
	int mLevel;
};