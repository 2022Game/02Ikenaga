#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

class CPortionRed : public CObjectBase
{
public:
	CPortionRed();
	~CPortionRed();

	void Update();
	void Render();

private:
	void CreateFieldObjects();

	//CModel* mpModel;

	CModel* mpPortionRed;
};