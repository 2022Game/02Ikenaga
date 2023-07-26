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
};

#endif