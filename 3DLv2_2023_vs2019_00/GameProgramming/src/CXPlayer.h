#pragma once
#ifndef CXPLAYER_H
#define CXPLAYER_H

#include"CXCharacter.h"
#include"CCollider.h"
#include "CInput.h"

class CXPlayer :public CXCharacter
{
public:
	void Init(CModelX* model);
	//コライダの初期化
	CXPlayer()
		:mColSphereBody(this, nullptr, CVector(), 0.5f)
		, mColSphereHead(this, nullptr, CVector(0.0f, 5.0f, -3.0f), 0.5f)
		, mColSphereSword(this, nullptr, CVector(-10.0f, 10.0f, 50.0f), 0.3f)
	{
	}
	void Update();
private:
	CCollider mColSphereHead;  //頭
	CCollider mColSphereSword;  //剣
	//コライダの宣言
	CCollider mColSphereBody;  //体
	CInput mInput;
};

#endif
