#pragma once
#ifndef CKOKKI_H
#define CKOKKI_H
#include"CCharacter3.h"
#include"CCollider.h"
#include "CColliderMesh.h"

class CKokki :public CCharacter3
{
public:
	void Collision();
	void Collision(CCollider* m, CCollider* o);
	CKokki(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale);
	void Update();
private:
	CModel mStage;
	CColliderMesh mColliderMesh1;
	//CPlayer *mpPlayer;
	//ƒRƒ‰ƒCƒ_
	CCollider mCollider1;
	CCollider mCollider2;
	CCollider mCollider3;
	//CMatrix *mMatrix;
};

#endif