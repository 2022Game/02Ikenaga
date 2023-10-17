#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

/*
経験値クラス
オブジェクトベース継承
*/
class CBall : public CObjectBase
{
public:
	CBall();
	~CBall();

	void Update();
	void Render();

private:
	void CreateFieldObjects();

	CModel* mpBall;
};