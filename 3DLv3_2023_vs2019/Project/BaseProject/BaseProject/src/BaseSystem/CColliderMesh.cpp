#include "CColliderMesh.h"

CColliderMesh::CColliderMesh()
	: mpColliderTriangles(nullptr)
{
}

CColliderMesh::~CColliderMesh()
{
	if (mpColliderTriangles != nullptr)
	{	//三角コライダ配列の削除
		delete[] mpColliderTriangles;
	}
}

//Set(親, 親行列, モデル)
//モデルから三角コライダの生成
void CColliderMesh::Set(CObjectBase* owner, ELayer layer, CModel* model)
{
	auto triangles = model->Triangles();
	mColliderTriangleCount = triangles.size();
	//モデルの三角ポリゴンで三角コライダの配列作成
	mpColliderTriangles = new CColliderTriangle[mColliderTriangleCount];
	for (int i = 0; i < mColliderTriangleCount; i++)
	{
		auto& tri = triangles[i];
		//三角コライダの設定
		mpColliderTriangles[i].Set
		(
			owner, layer,
			tri.V0(), tri.V1(), tri.V2()
		);
	}
}
