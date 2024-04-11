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

	// 死亡処理(レベル50以下の敵用)
	virtual void Death();

	// 死亡処理(レベル90以下の敵用)
	virtual void Death2();
protected:
	CHpGauge* mpHpGauge;      // HPゲージ
private:
};