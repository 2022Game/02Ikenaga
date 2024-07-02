#include "CField.h"
#include "CCollisionManager.h"
#include "CMoveFloor.h"
#include "CRotateFloor.h"
#include "CLineEffect.h"

// �R���X�g���N
CField::CField()
	: CObjectBase(ETag::eField, ETaskPriority::eBackground)
	, mEffectAnimData(1, 11, true, 11, 0.03f)
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
	//// �d���G�t�F�N�g
	//CLineEffect* le = new CLineEffect(ETag::eField);
	//le->SetTexture("Current");
	////le->SetBaseUV(CRect(0.0f, 0.0f, 128.0f, 1024.0f));
	////le->SetAnimData(&mEffectAnimData);

	//CVector startPos = CVector(50.0f, 10.0f, 0.0f);
	//CVector endPos = CVector(50.0f, 10.0f, 150.0f);
	//int div = 3;
	//float width = 5.0f;
	//le->AddPoint(startPos, width, width);
	//for (int i = 0; i < div; i++)
	//{
	//	float alpha = (float)(i + 1) / div;
	//	CVector pos = CVector::Lerp(startPos, endPos, alpha);
	//	le->AddPoint(pos, width, width);
	//}
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
