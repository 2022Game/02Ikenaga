#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"
#include "CWeapon.h"

/*
盾クラス
オブジェクトベース継承
*/
class CShield : public CWeapon
{
public:
	CShield();
	~CShield();

	void Update();
	void Render();

private:

	CModel* mpShield;
};
