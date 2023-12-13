#include "CExpManager.h"
#include "Maths.h"

// 生成する経験値オーブの最小数
#define SPAWN_EXP_COUNT_MIN 1
// 生成する経験値オーブの最大数
#define SPAWN_EXP_COUNT_MAX 2

CExpManeger* CExpManeger::spInstance = nullptr;

// コンストラクタ
CExpManeger::CExpManeger()
{
	spInstance = this;
}


//デストラクタ
CExpManeger::~CExpManeger()
{
	spInstance = nullptr;
}

// 経験値を作成
void CExpManeger::SpawnExp(int exp, const CVector& pos)
{
	if (exp == 0)return;

	// 経験値オーブの生成数を設定
	int spawnCount = exp;
	// 生成数が最小数より大きい場合は、
	if (spawnCount > SPAWN_EXP_COUNT_MIN)
	{
		// 生成できる最大数を取得
		int maxCount = std::min(spawnCount, SPAWN_EXP_COUNT_MAX);
		// 生成数を最小数～最大数のランダムで取得
		spawnCount = Math::Rand(SPAWN_EXP_COUNT_MIN, maxCount);
	}

	// 開始角度はランダムに決める
	float startAngle = Math::Rand(0.0f, M_PI * 2.0f);
	// 加算する経験値のベース値
	int baseExp = exp / spawnCount;
	// 割り切れない余りの経験値
	int remainExp = exp % spawnCount;
	for (int i = 0; i < spawnCount; i++)
	{
		// 余りの経験値が残っている場合は、加算する経験値のベース値+1する
		CExp* e = new CExp(baseExp + (i < remainExp ? 1 : 0));

		// 等間隔に360度経験値を出現させる
		float per = (float)i / spawnCount;
		float angle = startAngle + M_PI * 2.0f * per;
		CVector dir = CVector(cosf(angle), 0.0f, sinf(angle));
		CVector epos = pos + dir * 1.0f;

		e->Setup(epos, dir);
		e->Scale(3.0f, 3.0f, 3.0f);
		e->SetColor(CColor(0.0f, 1.0f, 0.2, 1.0f));
	}

}