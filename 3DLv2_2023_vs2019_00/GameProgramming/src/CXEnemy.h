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
	//CXPlayer mXPlayer;
	CCollider mColSphereHead;  //��
	CCollider mColSphereSword;  //��
	//�R���C�_�̐錾
	CCollider mColSphereBody;  //��
};

#endif