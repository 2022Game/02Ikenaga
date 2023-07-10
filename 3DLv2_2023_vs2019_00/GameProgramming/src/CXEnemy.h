#pragma once
#ifndef CXENEMY_H
#define CXENEMY_H

#include"CXCharacter.h"
#include "CCollider.h"
#include "CXPlayer.h"

class CXEnemy :public CXCharacter
{
public:
	//衝突処理
	void Collision(CCollider* m, CCollider* o);
	void Init(CModelX* model);
	//コライダの初期化
	CXEnemy();
private:
	CCollider mColSphereSword0;  //剣
	CCollider mColSphereSword1;  //剣
	CCollider mColSphereSword2;  //剣
	CCollider mColSphereHead;  //頭
	//コライダの宣言
	CCollider mColSphereBody;  //体
};

#endif