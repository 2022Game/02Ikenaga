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
	bool PortionBlue = false;
	bool PortionGreen = false;
	bool PortionRed = false;

	float rand2 = Math::Rand(0, 3);
	float rand3 = Math::Rand(4, 7);

	if (rand2 == 3)PortionGreen = true;
	if (rand3 == 7) PortionRed = true;

	if (PortionGreen)
	{
		// 回復ポーション
		CPortionGreen* portiongreen = new CPortionGreen();
		portiongreen->Position(pos);
		portiongreen->Scale(200.0f, 200.0f, 200.0f);
	}
	else if (PortionRed)
	{
		// 攻撃力アップポーション
		CPortionRed* portionred = new CPortionRed();
		portionred->Position(pos);
		portionred->Scale(200.0f, 200.0f, 200.0f);
	}
	else
	{
		// 防御力アップポーション
		CPortionBlue* portionblue = new CPortionBlue();
		portionblue->Position(pos);
		portionblue->Scale(200.0f, 200.0f, 200.0f);
	}
}