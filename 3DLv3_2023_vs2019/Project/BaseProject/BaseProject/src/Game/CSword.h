#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

/*
剣クラス
オブジェクトベース継承
*/
class CSword : public CObjectBase
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

