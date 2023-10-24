#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"
#include "CWeapon.h"

/*
剣クラス
オブジェクトベース継承
*/
class CSword : public CWeapon
{
public:
	CSword();
	~CSword();

	void Update();
	void Render();

private:
	void CreateFieldObjects();

	CModel* mpSword;
};

