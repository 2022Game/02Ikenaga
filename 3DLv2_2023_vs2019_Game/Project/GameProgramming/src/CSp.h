#pragma once
#pragma once
#ifndef CSP_H
#define CSP_H
#include"CCharacter3.h"
#include"CCollider.h"
#include "CColliderMesh.h"

class CSp :public CCharacter3
{
public:
	void Collision();
	void Collision(CCollider* m, CCollider* o);
	CSp(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale);
	void Update();
private:
	CModel mStage;
	//ƒRƒ‰ƒCƒ_
	//CCollider mCollider1;
};

#endif
