#include "CShield.h"
#include "CCollisionManager.h"

// �R���X�g���N
CShield::CShield()
{
	mpShield = CResourceManager::Get<CModel>("Shield");
}

// �f�X�g���N�^
CShield::~CShield()
{
}

// �X�V
void CShield::Update()
{
}

// �`��
void CShield::Render()
{
	if (mpAttachMtx == nullptr)
	{
		mpShield->Render(Matrix());
	}
	else
	{
		CMatrix sm;
		sm.Scale(100.0f, 100.0f, 100.0f);
		mpShield->Render(sm * (*mpAttachMtx));
	}
}