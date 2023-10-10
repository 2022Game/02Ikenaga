#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

class CBall : public CObjectBase
{
public:
	CBall();
	~CBall();

	void Update();
	void Render();

private:
	void CreateFieldObjects();

	//CModel* mpModel;

	CModel* mpBall;
};