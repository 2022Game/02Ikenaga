#pragma once
#include "CTask.h"
#include "CModel.h"
#include "CColliderMesh.h"

class CField :public CTask
{
public:
	//コンストラクタ
	CField();
	//デストラクタ
	~CField();

	void Update();
	void Render();

private:
	CModel* mModel;  //フィールドのモデルデータ
	CColliderMesh mColliderMesh;
};