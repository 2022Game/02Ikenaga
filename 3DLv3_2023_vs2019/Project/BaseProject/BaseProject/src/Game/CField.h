#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

/*
 フィールド
*/
class CField : public CObjectBase
{
public:
	// コンストラク
	CField();
	//　デストラクタ
	~CField();

	// コライダーを取得
	CCollider* GetCollider() const;

	// 更新
	void Update();
	// 描画
	void Render();

private:
	void CreateFieldObjects();

	CModel* mpModel;
	CColliderMesh* mpColliderMesh;

	CModel* mpCubeModel;
	CModel* mpCylinderModel;
};