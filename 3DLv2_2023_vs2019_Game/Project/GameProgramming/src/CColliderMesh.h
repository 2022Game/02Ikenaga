#ifndef CCOLLIDERMESH_H
#define CCOLLIDERMESH_H
#include "CModel.h"
#include "CColliderTriangle.h"
#include "CCollider.h"

/*
モデルデータから三角コライダの生成
*/
class CColliderMesh
{
public:
	void ChangePriority();
	CColliderMesh();
	~CColliderMesh();
	//Set（親、親行列、モデル）
	//モデルから三角コライダの生成
	void Set(CCharacter3* parent, CMatrix* matrix, CModel* model);
	//描画するかを設定
	void SetHidden(bool isHidden);
private:
	//三角コライダの配列生成
	CColliderTriangle* mpColliderTriangles;
	//三角コライダの配列の要素数
	int mTrianglesCount;
};
#endif