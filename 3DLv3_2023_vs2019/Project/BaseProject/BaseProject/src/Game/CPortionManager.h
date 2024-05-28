#pragma once
#include "CPortionBlue.h"
#include "CPortionGreen.h"
#include "CPortionRed.h"

class CPortionManager : public CTask
{
public:

	// ランダムでポーション生成
	static void  RandomPortion(const CVector& pos);

private:
	// コンストラク
	CPortionManager();
	// デストラクタ
	~CPortionManager();
};