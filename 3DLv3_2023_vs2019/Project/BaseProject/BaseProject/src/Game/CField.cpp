#include "CField.h"
#include "CCollisionManager.h"
#include "CMoveFloor.h"
#include "CRotateFloor.h"

// �R���X�g���N
CField::CField()
	: CObjectBase(ETag::eField, ETaskPriority::eBackground)
{
	mpModel = CResourceManager::Get<CModel>("Field");

	mpColliderMesh = new CColliderMesh(this, ELayer::eField, mpModel, true);

	CreateFieldObjects();
}

// �f�X�g���N�^
CField::~CField()
{
	SAFE_DELETE(mpColliderMesh);
}

void CField::CreateFieldObjects()
{
	/*mpCubeModel = new CModel();
	mpCubeModel->Load("Field\\Object\\cube.obj", "Field\\Object\\cube.mtl");

	mpCylinderModel = new CModel();
	mpCylinderModel->Load("Field\\Object\\cylinder.obj", "Field\\Object\\cylinder.mtl");*/

	//// �����Ȃ����A
	//new CMoveFloor
	//(
	//	mpCubeModel,
	//	CVector(135.0f, 70.0f, -52.5f), CVector(0.25f, 1.0f, 0.25f),
	//	CVector(0.0f, 50.0f, 0.0f), 5.0f
	//);
}

CCollider* CField::GetCollider() const
{
	return mpColliderMesh;
}

// �X�V
void CField::Update()
{
}

// �`��
void CField::Render()
{
	mpModel->Render(Matrix());
}
