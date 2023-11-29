#pragma once
#include "CXCharacter.h"

// エネミーのベースクラス
class  CEnemy :public CXCharacter
{
public:
	//　コンストラクタ
	CEnemy();
	// デストラクタ
	virtual ~CEnemy();

	// 死亡処理
	virtual void Death();

private:
};