#include "CExpManager.h"
#include "Maths.h"

// 小さいスライム用
#define Slime_SPAWN_EXP_COUNT_MIN 1  // 生成する経験値オーブの最小数
#define Slime_SPAWN_EXP_COUNT_MAX 2  // 生成する経験値オーブの最大数
// 大きいスライム用
#define SlimeBig_SPAWN_EXP_COUNT_MIN 3  // 生成する経験値オーブの最小数
#define SlimeBig_SPAWN_EXP_COUNT_MAX 4  // 生成する経験値オーブの最大数

// 小さいマッシュルーム用
#define Mushroom_SPAWN_EXP_COUNT_MIN 2  // 生成する経験値オーブの最小数
#define Mushroom_SPAWN_EXP_COUNT_MAX 3  // 生成する経験値オーブの最大数
// 大きいマッシュルーム用
#define MushroomBig_SPAWN_EXP_COUNT_MIN 4  // 生成する経験値オーブの最小数
#define MushroomBig_SPAWN_EXP_COUNT_MAX 5  // 生成する経験値オーブの最大数

// 小さい亀用
#define Turtle_SPAWN_EXP_COUNT_MIN 3  // 生成する経験値オーブの最小数
#define Turtle_SPAWN_EXP_COUNT_MAX 4  // 生成する経験値オーブの最大数
// 大きい亀用
#define TurtleBig_SPAWN_EXP_COUNT_MIN 5  // 生成する経験値オーブの最小数
#define TurtleBig_SPAWN_EXP_COUNT_MAX 6  // 生成する経験値オーブの最大数

// レベル50以下の敵用
#define Level50_SPAWN_EXP_COUNT_MIN 2  // 生成する経験値オーブの最小数
#define Level50_SPAWN_EXP_COUNT_MAX 3  // 生成する経験値オーブの最大数

// レベル90以下の敵用
#define Level90_SPAWN_EXP_COUNT_MIN 5  // 生成する経験値オーブの最小数
#define Level90_SPAWN_EXP_COUNT_MAX 7  // 生成する経験値オーブの最大数

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

// 経験値を作成(小さいスライム用)
void CExpManeger::SlimeSpawnExp(int exp, const CVector& pos)
{
	if (exp == 0)return;

	// 経験値オーブの生成数を設定
	int spawnCount = exp;
	// 生成数が最小数より大きい場合は
	if (spawnCount > Slime_SPAWN_EXP_COUNT_MIN)
	{
		// 生成できる最大数を取得
		int maxCount = std::min(spawnCount, Slime_SPAWN_EXP_COUNT_MAX);
		// 生成数を最小数～最大数のランダムで取得
		spawnCount = Math::Rand(Slime_SPAWN_EXP_COUNT_MIN, maxCount);
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
// 経験値を作成(大きいスライム用)
void CExpManeger::SlimeBigSpawnExp(int exp, const CVector& pos)
{
	if (exp == 0)return;

	// 経験値オーブの生成数を設定
	int spawnCount = exp;
	// 生成数が最小数より大きい場合は
	if (spawnCount > SlimeBig_SPAWN_EXP_COUNT_MIN)
	{
		// 生成できる最大数を取得
		int maxCount = std::min(spawnCount, SlimeBig_SPAWN_EXP_COUNT_MAX);
		// 生成数を最小数～最大数のランダムで取得
		spawnCount = Math::Rand(SlimeBig_SPAWN_EXP_COUNT_MIN, maxCount);
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

// 経験値を作成(小さいマッシュルーム用)
void CExpManeger::MushroomSpawnExp(int exp, const CVector& pos)
{
	if (exp == 0)return;

	// 経験値オーブの生成数を設定
	int spawnCount = exp;
	// 生成数が最小数より大きい場合は
	if (spawnCount > Mushroom_SPAWN_EXP_COUNT_MIN)
	{
		// 生成できる最大数を取得
		int maxCount = std::min(spawnCount, Mushroom_SPAWN_EXP_COUNT_MAX);
		// 生成数を最小数～最大数のランダムで取得
		spawnCount = Math::Rand(Mushroom_SPAWN_EXP_COUNT_MIN, maxCount);
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
// 経験値を作成(大きいマッシュルーム用)
void CExpManeger::MushroomBigSpawnExp(int exp, const CVector& pos)
{
	if (exp == 0)return;

	// 経験値オーブの生成数を設定
	int spawnCount = exp;
	// 生成数が最小数より大きい場合は
	if (spawnCount > MushroomBig_SPAWN_EXP_COUNT_MIN)
	{
		// 生成できる最大数を取得
		int maxCount = std::min(spawnCount, MushroomBig_SPAWN_EXP_COUNT_MAX);
		// 生成数を最小数～最大数のランダムで取得
		spawnCount = Math::Rand(MushroomBig_SPAWN_EXP_COUNT_MIN, maxCount);
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

// 経験値を作成(小さい亀用)
void CExpManeger::TurtleSpawnExp(int exp, const CVector& pos)
{
	if (exp == 0)return;

	// 経験値オーブの生成数を設定
	int spawnCount = exp;
	// 生成数が最小数より大きい場合は
	if (spawnCount > Turtle_SPAWN_EXP_COUNT_MIN)
	{
		// 生成できる最大数を取得
		int maxCount = std::min(spawnCount, Turtle_SPAWN_EXP_COUNT_MAX);
		// 生成数を最小数～最大数のランダムで取得
		spawnCount = Math::Rand(Turtle_SPAWN_EXP_COUNT_MIN, maxCount);
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
// 経験値を作成(大きい亀用)
void CExpManeger::TurtleBigSpawnExp(int exp, const CVector& pos)
{
	if (exp == 0)return;

	// 経験値オーブの生成数を設定
	int spawnCount = exp;
	// 生成数が最小数より大きい場合は
	if (spawnCount > TurtleBig_SPAWN_EXP_COUNT_MIN)
	{
		// 生成できる最大数を取得
		int maxCount = std::min(spawnCount, TurtleBig_SPAWN_EXP_COUNT_MAX);
		// 生成数を最小数～最大数のランダムで取得
		spawnCount = Math::Rand(TurtleBig_SPAWN_EXP_COUNT_MIN, maxCount);
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

// 経験値を作成(レベル50以下の敵用)
void CExpManeger::SpawnExp(int exp, const CVector& pos)
{
	if (exp == 0)return;

	// 経験値オーブの生成数を設定
	int spawnCount = exp;
	// 生成数が最小数より大きい場合は、
	if (spawnCount > Level50_SPAWN_EXP_COUNT_MIN)
	{
		// 生成できる最大数を取得
		int maxCount = std::min(spawnCount, Level50_SPAWN_EXP_COUNT_MAX);
		// 生成数を最小数～最大数のランダムで取得
		spawnCount = Math::Rand(Level50_SPAWN_EXP_COUNT_MIN, maxCount);
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

// 経験値を作成(レベル90以下の敵用)
void CExpManeger::SpawnExp2(int exp, const CVector& pos)
{
	if (exp == 0)return;

	// 経験値オーブの生成数を設定
	int spawnCount = exp;
	// 生成数が最小数より大きい場合は、
	if (spawnCount > Level90_SPAWN_EXP_COUNT_MIN)
	{
		// 生成できる最大数を取得
		int maxCount = std::min(spawnCount, Level90_SPAWN_EXP_COUNT_MAX);
		// 生成数を最小数～最大数のランダムで取得
		spawnCount = Math::Rand(Level90_SPAWN_EXP_COUNT_MIN, maxCount);
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