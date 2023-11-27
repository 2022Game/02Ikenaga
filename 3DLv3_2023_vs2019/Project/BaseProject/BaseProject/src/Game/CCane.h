#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"
#include "CWeapon.h"

/*
杖クラス
オブジェクトベース継承
*/
class CCane : public CWeapon
{
public:
	CCane();
	~CCane();

	void Update();
	void Render();

private:

	CModel* mpCane;
};
