#include "CExpManager.h"
#include "Maths.h"

// 小さいスライム用
#define Slime_SPAWN_EXP_COUNT_MIN 1     // 生成する経験値オーブの最小数
#define Slime_SPAWN_EXP_COUNT_MAX 2     // 生成する経験値オーブの最大数
// 大きいスライム用
#define SlimeBig_SPAWN_EXP_COUNT_MIN 3  // 生成する経験値オーブの最小数
#define SlimeBig_SPAWN_EXP_COUNT_MAX 4  // 生成する経験値オーブの最大数

// 小さいマッシュルーム用
#define Mushroom_SPAWN_EXP_COUNT_MIN 2     // 生成する経験値オーブの最小数
#define Mushroom_SPAWN_EXP_COUNT_MAX 3     // 生成する経験値オーブの最大数
// 大きいマッシュルーム用
#define MushroomBig_SPAWN_EXP_COUNT_MIN 4  // 生成する経験値オーブの最小数
#define MushroomBig_SPAWN_EXP_COUNT_MAX 5  // 生成する経験値オーブの最大数

// 小さい亀用
#define Turtle_SPAWN_EXP_COUNT_MIN 3     // 生成する経験値オーブの最小数
#define Turtle_SPAWN_EXP_COUNT_MAX 4     // 生成する経験値オーブの最大数
// 大きい亀用
#define TurtleBig_SPAWN_EXP_COUNT_MIN 5  // 生成する経験値オーブの最小数
#define TurtleBig_SPAWN_EXP_COUNT_MAX 6  // 生成する経験値オーブの最大数

// 小さいエイ用
#define Ray_SPAWN_EXP_COUNT_MIN 4     // 生成する経験値オーブの最小数
#define Ray_SPAWN_EXP_COUNT_MAX 5     // 生成する経験値オーブの最大数
// 大きいエイ用
#define RayBig_SPAWN_EXP_COUNT_MIN 6  // 生成する経験値オーブの最小数
#define RayBig_SPAWN_EXP_COUNT_MAX 7  // 生成する経験値オーブの最大数

// 小さい蜂用
#define Bee_SPAWN_EXP_COUNT_MIN 5     // 生成する経験値オーブの最小数
#define Bee_SPAWN_EXP_COUNT_MAX 6     // 生成する経験値オーブの最大数
// 大きい蜂用
#define BeeBig_SPAWN_EXP_COUNT_MIN 7  // 生成する経験値オーブの最小数
#define BeeBig_SPAWN_EXP_COUNT_MAX 8  // 生成する経験値オーブの最大数

// 小さいサボテン用
#define Cactus_SPAWN_EXP_COUNT_MIN 6     // 生成する経験値オーブの最小数
#define Cactus_SPAWN_EXP_COUNT_MAX 7     // 生成する経験値オーブの最大数
// 大きいサボテン用
#define CactusBig_SPAWN_EXP_COUNT_MIN 8  // 生成する経験値オーブの最小数
#define CactusBig_SPAWN_EXP_COUNT_MAX 9  // 生成する経験値オーブの最大数

// 小さいチェストモンスター用
#define Chest_SPAWN_EXP_COUNT_MIN 7      // 生成する経験値オーブの最小数
#define Chest_SPAWN_EXP_COUNT_MAX 8      // 生成する経験値オーブの最大数
// 大きいチェストモンスター用
#define ChestBig_SPAWN_EXP_COUNT_MIN 9   // 生成する経験値オーブの最小数
#define ChestBig_SPAWN_EXP_COUNT_MAX 10  // 生成する経験値オーブの最大数

// 小さい球体モンスター用
#define Beholder_SPAWN_EXP_COUNT_MIN 8      // 生成する経験値オーブの最小数
#define Beholder_SPAWN_EXP_COUNT_MAX 9      // 生成する経験値オーブの最大数
// 大きい球体モンスター用
#define BeholderBig_SPAWN_EXP_COUNT_MIN 10  // 生成する経験値オーブの最小数
#define BeholderBig_SPAWN_EXP_COUNT_MAX 11  // 生成する経験値オーブの最大数

// 小さいボクサー用
#define Boxer_SPAWN_EXP_COUNT_MIN 9      // 生成する経験値オーブの最小数
#define Boxer_SPAWN_EXP_COUNT_MAX 10     // 生成する経験値オーブの最大数
// 大きいボクサー用
#define BoxerBig_SPAWN_EXP_COUNT_MIN 11  // 生成する経験値オーブの最小数
#define BoxerBig_SPAWN_EXP_COUNT_MAX 12  // 生成する経験値オーブの最大数

// ドラゴン用
#define Dragon_SPAWN_EXP_COUNT_MIN 30   // 生成する経験値オーブの最小数
#define Dragon_SPAWN_EXP_COUNT_MAX 31   // 生成する経験値オーブの最大数

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
		// 生成数を最小数〜最大数のランダムで取得
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
		// 生成数を最小数〜最大数のランダムで取得
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
		// 生成数を最小数〜最大数のランダムで取得
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
		// 生成数を最小数〜最大数のランダムで取得
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
		// 生成数を最小数〜最大数のランダムで取得
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
		// 生成数を最小数〜最大数のランダムで取得
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

// 経験値を作成(小さいエイ用)
void CExpManeger::RaySpawnExp(int exp, const CVector& pos)
{
	if (exp == 0)return;

	// 経験値オーブの生成数を設定
	int spawnCount = exp;
	// 生成数が最小数より大きい場合は
	if (spawnCount > Ray_SPAWN_EXP_COUNT_MIN)
	{
		// 生成できる最大数を取得
		int maxCount = std::min(spawnCount, Ray_SPAWN_EXP_COUNT_MAX);
		// 生成数を最小数〜最大数のランダムで取得
		spawnCount = Math::Rand(Ray_SPAWN_EXP_COUNT_MIN, maxCount);
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
// 経験値を作成(大きいエイ用)
void CExpManeger::RayBigSpawnExp(int exp, const CVector& pos)
{
	if (exp == 0)return;

	// 経験値オーブの生成数を設定
	int spawnCount = exp;
	// 生成数が最小数より大きい場合は
	if (spawnCount > RayBig_SPAWN_EXP_COUNT_MIN)
	{
		// 生成できる最大数を取得
		int maxCount = std::min(spawnCount, RayBig_SPAWN_EXP_COUNT_MAX);
		// 生成数を最小数〜最大数のランダムで取得
		spawnCount = Math::Rand(RayBig_SPAWN_EXP_COUNT_MIN, maxCount);
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

// 経験値を作成(小さい蜂用)
void CExpManeger::BeeSpawnExp(int exp, const CVector& pos)
{
	if (exp == 0)return;

	// 経験値オーブの生成数を設定
	int spawnCount = exp;
	// 生成数が最小数より大きい場合は
	if (spawnCount > Bee_SPAWN_EXP_COUNT_MIN)
	{
		// 生成できる最大数を取得
		int maxCount = std::min(spawnCount, Bee_SPAWN_EXP_COUNT_MAX);
		// 生成数を最小数〜最大数のランダムで取得
		spawnCount = Math::Rand(Bee_SPAWN_EXP_COUNT_MIN, maxCount);
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
// 経験値を作成(大きい蜂用)
void CExpManeger::BeeBigSpawnExp(int exp, const CVector& pos)
{
	if (exp == 0)return;

	// 経験値オーブの生成数を設定
	int spawnCount = exp;
	// 生成数が最小数より大きい場合は
	if (spawnCount > BeeBig_SPAWN_EXP_COUNT_MIN)
	{
		// 生成できる最大数を取得
		int maxCount = std::min(spawnCount, BeeBig_SPAWN_EXP_COUNT_MAX);
		// 生成数を最小数〜最大数のランダムで取得
		spawnCount = Math::Rand(BeeBig_SPAWN_EXP_COUNT_MIN, maxCount);
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

// 経験値を作成(小さいサボテン用)
void CExpManeger::CactusSpawnExp(int exp, const CVector& pos)
{
	if (exp == 0)return;

	// 経験値オーブの生成数を設定
	int spawnCount = exp;
	// 生成数が最小数より大きい場合は
	if (spawnCount > Cactus_SPAWN_EXP_COUNT_MIN)
	{
		// 生成できる最大数を取得
		int maxCount = std::min(spawnCount, Cactus_SPAWN_EXP_COUNT_MAX);
		// 生成数を最小数〜最大数のランダムで取得
		spawnCount = Math::Rand(Cactus_SPAWN_EXP_COUNT_MIN, maxCount);
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
// 経験値を作成(大きいサボテン用)
void CExpManeger::CactusBigSpawnExp(int exp, const CVector& pos)
{
	if (exp == 0)return;

	// 経験値オーブの生成数を設定
	int spawnCount = exp;
	// 生成数が最小数より大きい場合は
	if (spawnCount > CactusBig_SPAWN_EXP_COUNT_MIN)
	{
		// 生成できる最大数を取得
		int maxCount = std::min(spawnCount, CactusBig_SPAWN_EXP_COUNT_MAX);
		// 生成数を最小数〜最大数のランダムで取得
		spawnCount = Math::Rand(CactusBig_SPAWN_EXP_COUNT_MIN, maxCount);
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

// 経験値を作成(小さいチェストモンスター用)
void CExpManeger::ChestSpawnExp(int exp, const CVector& pos)
{
	if (exp == 0)return;

	// 経験値オーブの生成数を設定
	int spawnCount = exp;
	// 生成数が最小数より大きい場合は
	if (spawnCount > Chest_SPAWN_EXP_COUNT_MIN)
	{
		// 生成できる最大数を取得
		int maxCount = std::min(spawnCount, Chest_SPAWN_EXP_COUNT_MAX);
		// 生成数を最小数〜最大数のランダムで取得
		spawnCount = Math::Rand(Chest_SPAWN_EXP_COUNT_MIN, maxCount);
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
// 経験値を作成(大きいチェストモンスター用)
void CExpManeger::ChestBigSpawnExp(int exp, const CVector& pos)
{
	if (exp == 0)return;

	// 経験値オーブの生成数を設定
	int spawnCount = exp;
	// 生成数が最小数より大きい場合は
	if (spawnCount > ChestBig_SPAWN_EXP_COUNT_MIN)
	{
		// 生成できる最大数を取得
		int maxCount = std::min(spawnCount, ChestBig_SPAWN_EXP_COUNT_MAX);
		// 生成数を最小数〜最大数のランダムで取得
		spawnCount = Math::Rand(ChestBig_SPAWN_EXP_COUNT_MIN, maxCount);
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

// 経験値を作成(小さい球体モンスター用)
void CExpManeger::BeholderSpawnExp(int exp, const CVector& pos)
{
	if (exp == 0)return;

	// 経験値オーブの生成数を設定
	int spawnCount = exp;
	// 生成数が最小数より大きい場合は
	if (spawnCount > Beholder_SPAWN_EXP_COUNT_MIN)
	{
		// 生成できる最大数を取得
		int maxCount = std::min(spawnCount, Beholder_SPAWN_EXP_COUNT_MAX);
		// 生成数を最小数〜最大数のランダムで取得
		spawnCount = Math::Rand(Beholder_SPAWN_EXP_COUNT_MIN, maxCount);
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
// 経験値を作成(大きい球体モンスター用)
void CExpManeger::BeholderBigSpawnExp(int exp, const CVector& pos)
{
	if (exp == 0)return;

	// 経験値オーブの生成数を設定
	int spawnCount = exp;
	// 生成数が最小数より大きい場合は
	if (spawnCount > BeholderBig_SPAWN_EXP_COUNT_MIN)
	{
		// 生成できる最大数を取得
		int maxCount = std::min(spawnCount, BeholderBig_SPAWN_EXP_COUNT_MAX);
		// 生成数を最小数〜最大数のランダムで取得
		spawnCount = Math::Rand(BeholderBig_SPAWN_EXP_COUNT_MIN, maxCount);
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

// 経験値を作成(小さいボクサー用)
void CExpManeger::BoxerSpawnExp(int exp, const CVector& pos)
{
	if (exp == 0)return;

	// 経験値オーブの生成数を設定
	int spawnCount = exp;
	// 生成数が最小数より大きい場合は
	if (spawnCount > Boxer_SPAWN_EXP_COUNT_MIN)
	{
		// 生成できる最大数を取得
		int maxCount = std::min(spawnCount, Boxer_SPAWN_EXP_COUNT_MAX);
		// 生成数を最小数〜最大数のランダムで取得
		spawnCount = Math::Rand(Boxer_SPAWN_EXP_COUNT_MIN, maxCount);
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
// 経験値を作成(大きいボクサー用)
void CExpManeger::BoxerBigSpawnExp(int exp, const CVector& pos)
{
	if (exp == 0)return;

	// 経験値オーブの生成数を設定
	int spawnCount = exp;
	// 生成数が最小数より大きい場合は
	if (spawnCount > BoxerBig_SPAWN_EXP_COUNT_MIN)
	{
		// 生成できる最大数を取得
		int maxCount = std::min(spawnCount, BoxerBig_SPAWN_EXP_COUNT_MAX);
		// 生成数を最小数〜最大数のランダムで取得
		spawnCount = Math::Rand(BoxerBig_SPAWN_EXP_COUNT_MIN, maxCount);
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

// 経験値を作成(ボスのドラゴン用)
void CExpManeger::DragonSpawnExp(int exp, const CVector& pos)
{
	if (exp == 0)return;

	// 経験値オーブの生成数を設定
	int spawnCount = exp;
	// 生成数が最小数より大きい場合は、
	if (spawnCount > Dragon_SPAWN_EXP_COUNT_MIN)
	{
		// 生成できる最大数を取得
		int maxCount = std::min(spawnCount, Dragon_SPAWN_EXP_COUNT_MAX);
		// 生成数を最小数〜最大数のランダムで取得
		spawnCount = Math::Rand(Dragon_SPAWN_EXP_COUNT_MIN, maxCount);
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