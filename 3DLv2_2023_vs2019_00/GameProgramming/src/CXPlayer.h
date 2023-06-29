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
	//�R���C�_�̏�����
	void Update();
private:
	CCollider mColSphereHead;  //��
	CCollider mColSphereSword;  //��
	//�R���C�_�̐錾
	CCollider mColSphereBody;  //��
	CInput mInput;
};

#endif
