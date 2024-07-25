#pragma once
#include "CUIBase.h"

class CImage;

// ロック2
class CLock2 : public CUIBase
{
public:
	// コンストラク
	CLock2(float x, float y);

	// デストラクタ
	~CLock2();

	// 更新
	void Update();

private:
	CImage* mpLock;
	int mLevel;
};