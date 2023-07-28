#pragma once
#ifndef CSTAGE_H
#define CSTAGE_H
#include"CCharacter3.h"
#include"CCollider.h"
#include "CColliderMesh.h"

class CStage:public CCharacter3
{
public:
	void Collision();
	void Collision(CCollider* m, CCollider* o);
	CStage(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale);
	void Update();
};

#endif