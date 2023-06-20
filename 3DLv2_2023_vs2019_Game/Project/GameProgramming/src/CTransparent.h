#pragma once
#ifndef CTRANSPARRENT_H
#define CTRANSPARRENT_H
#include"CCharacter3.h"
#include"CCollider.h"
#include "CColliderMesh.h"

/*
�����̕�
*/
class CTransparent :public CCharacter3
{
public:
	void Collision();
	void Collision(CCollider* m, CCollider* o);
	CTransparent(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale);
	void Update();
	void Render();
private:
	CModel mStage;
	CColliderMesh mColliderMesh1;
	//CPlayer *mpPlayer;
	//�R���C�_
	/*CCollider mCollider1;
	CCollider mCollider2;
	CCollider mCollider3;*/
	//CMatrix *mMatrix;
};

#endif
