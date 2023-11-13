#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

/*
移動速度アップのポーションクラス
オブジェクトベース継承
*/
class CPortionBlue : public CObjectBase
{
public:
	CPortionBlue();
	~CPortionBlue();

	void Update();
	void Render();

private:
	
	CModel* mpPortionBlue;
};
