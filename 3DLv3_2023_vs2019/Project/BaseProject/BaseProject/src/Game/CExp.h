#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

/*
経験値クラス
オブジェクトベース継承
*/
class CExp : public CObjectBase
{
public:
	CExp();
	~CExp();

	void Update();
	void Render();

private:
	CModel* mpExp;
};