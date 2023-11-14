#include "CColliderMesh.h"

CColliderMesh::CColliderMesh(CObjectBase* owner, ELayer layer, CModel* model,
	bool isKinematic, float weight)
	: CCollider(owner, layer, EColliderType::eMesh, isKinematic, weight)
{
	Set(model);
}

CColliderMesh::~CColliderMesh()
{
	mVertices.clear();
}

void CColliderMesh::Set(CModel* model)
{
	mVertices.clear();
	if (model == nullptr) return;

	auto triangles = model->Triangles();
	int count = triangles.size();
	for (auto& tri : triangles)
	{
		mVertices.push_back(STVertex(tri.V0(), tri.V1(), tri.V2()));
	}
}

void CColliderMesh::Get(std::list<STVertex>* tris) const
{
	tris->clear();
	CMatrix m = Matrix();
	for (auto& vertex : mVertices)
	{
		CVector v0 = vertex.V[0] * m;
		CVector v1 = vertex.V[1] * m;
		CVector v2 = vertex.V[2] * m;
		tris->push_back(STVertex(v0, v1, v2));
	}
}

void CColliderMesh::Render()
{
}
