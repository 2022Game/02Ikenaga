#pragma once
#ifndef CXENEMY_H
#define CXENEMY_H

#include"CXCharacter.h"
#include "CCollider.h"
#include "CXPlayer.h"

class CXEnemy :public CXCharacter
{
public:
	//�Փˏ���
	void Collision(CCollider* m, CCollider* o);
	void Init(CModelX* model);
	//�R���C�_�̏�����
	CXEnemy();
private:
	CCollider mColSphereSword0;  //��
	CCollider mColSphereSword1;  //��
	CCollider mColSphereSword2;  //��
	CCollider mColSphereHead;  //��
	//�R���C�_�̐錾
	CCollider mColSphereBody;  //��
};

#endif