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

	// 経験値を作成(小さいエイ用)
	static void RaySpawnExp(int exp, const CVector& pos);
	// 経験値を作成(大きいエイ用)
	static void RayBigSpawnExp(int exp, const CVector& pos);

	// 経験値を作成(小さい蜂用)
	static void BeeSpawnExp(int exp, const CVector& pos);
	// 経験値を作成(大きい蜂用)
	static void BeeBigSpawnExp(int exp, const CVector& pos);

	// 経験値を作成(小さいサボテン用)
	static void CactusSpawnExp(int exp, const CVector& pos);
	// 経験値を作成(大きいサボテン用)
	static void CactusBigSpawnExp(int exp, const CVector& pos);

	// 経験値を作成(小さいチェストモンスター用)
	static void ChestSpawnExp(int exp, const CVector& pos);
	// 経験値を作成(大きいチェストモンスター用)
	static void ChestBigSpawnExp(int exp, const CVector& pos);

	// 経験値を作成(小さい球体モンスター用)
	static void BeholderSpawnExp(int exp, const CVector& pos);
	// 経験値を作成(大きい球体モンスター用)
	static void BeholderBigSpawnExp(int exp, const CVector& pos);

	// 経験値を作成(小さいボクサー用)
	static void BoxerSpawnExp(int exp, const CVector& pos);
	// 経験値を作成(大きいボクサー用)
	static void BoxerBigSpawnExp(int exp, const CVector& pos);

	// 経験値を作成(ボスのドラゴン用)
	static void DragonSpawnExp(int exp, const CVector& pos);

private:
	// コンストラクタ
	CExpManeger();
	//デストラクタ
	~CExpManeger();

	static CExpManeger* spInstance;
};