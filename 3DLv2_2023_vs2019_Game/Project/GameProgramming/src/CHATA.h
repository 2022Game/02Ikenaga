#pragma once
#ifndef CHATA_H
#define CHATA_H
#include"CCharacter3.h"
#include"CCollider.h"
#include "CColliderMesh.h"

class CHATA :public CCharacter3
{
public:
	void Collision();
	void Collision(CCollider* m, CCollider* o);
	CHATA(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale);
	void Update();
private:
	CModel mStage;
	CColliderMesh mColliderMesh1;
	//CPlayer *mpPlayer;
	//ƒRƒ‰ƒCƒ_
	/*CCollider mCollider1;
	CCollider mCollider2;
	CCollider mCollider3;*/
	//CMatrix *mMatrix;
};

#endif
