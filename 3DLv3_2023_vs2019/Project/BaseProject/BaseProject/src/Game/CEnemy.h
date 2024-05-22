#pragma once
#include "CXCharacter.h"
#include "CHpGauge.h"

// エネミーのベースクラス
class  CEnemy :public CXCharacter
{
public:
	//　コンストラクタ
	CEnemy();
	// デストラクタ
	virtual ~CEnemy();

	// 小さいスライムの死亡処理
	virtual void SlimeDeath();
	// 大きいスライムの死亡処理
	virtual void SlimeBigDeath();

	// 小さいマッシュルームの死亡処理
	virtual void MushroomDeath();
	// 大きいマッシュルームの死亡処理
	virtual void MushroomBigDeath();

	// 小さい亀の死亡処理
	virtual void TurtleDeath();
	// 大きい亀の死亡処理
	virtual void TurtleBigDeath();

	// 小さいエイの死亡処理
	virtual void RayDeath();
	// 大きいエイの死亡処理
	virtual void RayBigDeath();

	// 小さい蜂の死亡処理
	virtual void BeeDeath();
	// 大きい蜂の死亡処理
	virtual void BeeBigDeath();

	// 小さいサボテンの死亡処理
	virtual void CactusDeath();
	// 大きいサボテンの死亡処理
	virtual void CactusBigDeath();

	// 小さいチェストモンスターの死亡処理
	virtual void ChestDeath();
	// 大きいチェストモンスターの死亡処理
	virtual void ChestBigDeath();

	// 小さい球体モンスターの死亡処理
	virtual void BeholderDeath();
	// 大きい球体モンスターの死亡処理
	virtual void BeholderBigDeath();

	// 小さいボクサーの死亡処理
	virtual void BoxerDeath();
	// 大きいボクサーの死亡処理
	virtual void BoxerBigDeath();

	// ボス(ドラゴン)の死亡処理
	virtual void DragonDeath();

	// 死亡処理
	virtual void Death();
protected:
	CHpGauge* mpHpGauge;      // HPゲージ
private:
};