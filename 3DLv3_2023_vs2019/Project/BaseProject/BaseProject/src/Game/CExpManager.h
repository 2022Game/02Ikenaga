#pragma once
#include "CExp.h"

/*
 敵が落とす経験値
*/
class CExpManeger :public CTask
{
public:
	// 経験値を作成(小さいスライム用)
	static void SlimeSpawnExp(int exp, const CVector& pos);
	// 経験値を作成(大きいスライム用)
	static void SlimeBigSpawnExp(int exp, const CVector& pos);

	// 経験値を作成(小さいマッシュルーム用)
	static void MushroomSpawnExp(int exp, const CVector& pos);
	// 経験値を作成(大きいマッシュルーム用)
	static void MushroomBigSpawnExp(int exp, const CVector& pos);

	// 経験値を作成(小さい亀用)
	static void TurtleSpawnExp(int exp, const CVector& pos);
	// 経験値を作成(大きい亀用)
	static void TurtleBigSpawnExp(int exp, const CVector& pos);

	// 経験値を作成(レベル50以下の敵用)
	static void SpawnExp(int exp, const CVector& pos);
	// 経験値を作成(レベル90以下の敵用)
	static void SpawnExp2(int exp, const CVector& pos);

private:
	// コンストラクタ
	CExpManeger();
	//デストラクタ
	~CExpManeger();

	static CExpManeger* spInstance;
};