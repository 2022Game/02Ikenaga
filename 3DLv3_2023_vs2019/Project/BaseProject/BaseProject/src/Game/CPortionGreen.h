#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

/*
回復のポーションクラス
オブジェクトベース継承
*/
class CPortionGreen : public CObjectBase
{
public:
	CPortionGreen();
	~CPortionGreen();

	void Update();
	void Render();

private:
	void CreateFieldObjects();

	CModel* mpPortionGreen;
};
