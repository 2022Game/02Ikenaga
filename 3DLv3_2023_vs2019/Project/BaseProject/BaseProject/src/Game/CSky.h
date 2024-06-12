#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

/*
 空
*/
class CSky : public CObjectBase
{
public:
	// コンストラク
	CSky();
	//　デストラクタ
	~CSky();

	// 更新
	void Update();
	// 描画
	void Render();

private:
	CModel* mpModel;
};