#pragma once
#include "CXCharacter.h"
#include "CHpGauge.h"

// 敵の種類
enum class EEnemyType
{
	None = -1,

	eBee,
	eBeholder,
	eBoxer,
	eCactus,
	eChest,
	eDragon,
	eMushroom,
	eRay,
	eRich,
	eSlime,
	eTurtle,

	Num
};

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

	// 敵の種類を取得
	EEnemyType GetType() const;

	// 召喚した敵の死亡処理
	virtual void DeathSummonEnemy(CEnemy* enemy);
	// 自身を召喚した敵を設定
	void SetSummoner(CEnemy* summoner);

protected:
	EEnemyType mType;     // 敵の種類
	CHpGauge* mpHpGauge;  // HPゲージ

	CEnemy* mpSummoner;   // 自分自身を召喚した敵キャラ
};