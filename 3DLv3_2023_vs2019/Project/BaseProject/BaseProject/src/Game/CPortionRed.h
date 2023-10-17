#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

/*
攻撃力アップのポーションクラス
オブジェクトベース継承
*/
class CPortionRed : public CObjectBase
{
public:
	CPortionRed();
	~CPortionRed();

	void Update();
	void Render();

private:
	void CreateFieldObjects();

	CModel* mpPortionRed;
};