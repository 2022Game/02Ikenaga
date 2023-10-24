#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

/*
盾クラス
オブジェクトベース継承
*/
class CShield : public CObjectBase
{
public:
	CShield();
	~CShield();

	void Update();
	void Render();

private:
	void CreateFieldObjects();

	CModel* mpShield;
};
