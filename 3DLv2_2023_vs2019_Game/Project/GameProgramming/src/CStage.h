#pragma once
#ifndef CSTAGE_H
#define CSTAGE_H
#include"CCharacter3.h"
#include"CCollider.h"

class CStage:public CCharacter3
{
public:
	void Collision();
	CStage(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale);
	void Update();
};

#endif