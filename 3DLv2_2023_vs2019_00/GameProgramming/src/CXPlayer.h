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
	CXPlayer();
	//コライダの初期化
	void Update();
private:
	CCollider mColSphereHead;  //頭
	CCollider mColSphereSword;  //剣
	//コライダの宣言
	CCollider mColSphereBody;  //体
	CInput mInput;
};

#endif
