#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

/*
 夜空
*/
class CNightSky : public CObjectBase
{
public:
	// コンストラク
	CNightSky();
	//　デストラクタ
	~CNightSky();

	// 更新
	void Update();
	// 描画
	void Render();

private:
	CModel* mpModel;
	float mElapsedTime;	  // 経過時間
};