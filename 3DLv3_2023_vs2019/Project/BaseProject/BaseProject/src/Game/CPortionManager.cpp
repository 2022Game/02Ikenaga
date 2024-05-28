#include "CPortionManager.h"
#include "Maths.h"

// コンストラク
CPortionManager::CPortionManager()
{
}

// デストラクタ
CPortionManager::~CPortionManager()
{
}

// ランダムでポーション生成
void CPortionManager::RandomPortion(const CVector& pos)
{
	float rand = Math::Rand(0,100);
	if (rand <= 30)
	{
		// 攻撃力アップポーション
		CPortionRed* portionred = new CPortionRed();
		portionred->Position(pos);
		portionred->Scale(200.0f, 200.0f, 200.0f);
	}
	else if (rand <= 60)
	{
		// 防御力アップポーション
		CPortionBlue* portionblue = new CPortionBlue();
		portionblue->Position(pos);
		portionblue->Scale(200.0f, 200.0f, 200.0f);
	}
	else if(rand <= 90)
	{
		// 回復ポーション
		CPortionGreen* portiongreen = new CPortionGreen();
		portiongreen->Position(pos);
		portiongreen->Scale(200.0f, 200.0f, 200.0f);
	}
}