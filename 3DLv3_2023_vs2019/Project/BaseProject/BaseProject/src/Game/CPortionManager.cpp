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
	bool PortionRed = false;
	bool PortionGreen = false;
	bool PortionBlue = false;

	float rand = Math::Rand(0, 5);
	//float rand2 = Math::Rand(0, 10);
	//float rand3 = Math::Rand(0, 10);

	if (rand == 1) PortionRed = true;
	if (rand == 3)PortionGreen = true;
	if (rand == 5)PortionBlue = true;

	if (PortionRed)
	{
		PortionBlue = false;
		PortionGreen = false;
		// 攻撃力アップポーション
		CPortionRed* portionred = new CPortionRed();
		portionred->Position(pos);
		portionred->Scale(200.0f, 200.0f, 200.0f);
	}
	else if (PortionGreen)
	{
		PortionBlue = false;
		PortionRed = false;
		// 回復ポーション
		CPortionGreen* portiongreen = new CPortionGreen();
		portiongreen->Position(pos);
		portiongreen->Scale(200.0f, 200.0f, 200.0f);
	}
	else if (PortionBlue)
	{
		PortionGreen = false;
		PortionRed = false;
		// 防御力アップポーション
		CPortionBlue* portionblue = new CPortionBlue();
		portionblue->Position(pos);
		portionblue->Scale(200.0f, 200.0f, 200.0f);
	}
}