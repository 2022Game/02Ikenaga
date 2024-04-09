#pragma once
#include "CXCharacter.h"
#include "CHpGauge.h"

// エネミーのベースクラス(レベル50までの敵)
class  CEnemy :public CXCharacter
{
public:
	//　コンストラクタ
	CEnemy();
	// デストラクタ
	virtual ~CEnemy();

	// 死亡処理
	virtual void Death();
protected:
	CHpGauge* mpHpGauge;      // HPゲージ
private:
};