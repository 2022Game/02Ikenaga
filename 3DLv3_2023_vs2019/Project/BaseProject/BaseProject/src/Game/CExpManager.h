#pragma once
#include "CExp.h"

class CExpManeger :public CTask
{
public:
	// 経験値を作成
	static void SpawnExp(int exp, const CVector& pos);

private:
	// コンストラクタ
	CExpManeger();
	//デストラクタ
	~CExpManeger();

	static CExpManeger* spInstance;
};