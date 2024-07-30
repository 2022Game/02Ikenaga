#include "CField.h"
#include "CCollisionManager.h"
#include "CMoveFloor.h"
#include "CRotateFloor.h"
#include "CLineEffect.h"

// �R���X�g���N
CField::CField()
	: CObjectBase(ETag::eField, ETaskPriority::eBackground)
	//, mEffectAnimData(1, 11, true, 11, 0.03f)
{
	mpModel = CResourceManager::Get<CModel>("Field");

	mpColliderMesh = new CColliderMesh(this, ELayer::eField, mpModel, true);
}

// �f�X�g���N�^
CField::~CField()
{
	SAFE_DELETE(mpColliderMesh);
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
