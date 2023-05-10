#pragma once
#ifndef CSTAGE_H
#define CSTAGE_H
#include"CCharacter3.h"
#include"CCollider.h"
#include"CPlayer.h"

class CStage:public CCharacter3
{
public:
	void Collision();
	CStage(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale);
	void Update();
private:
	//CPlayer *mpPlayer;
	//ƒRƒ‰ƒCƒ_
	CCollider mCollider1;
	CCollider mCollider2;
	CCollider mCollider3;
};

#endif