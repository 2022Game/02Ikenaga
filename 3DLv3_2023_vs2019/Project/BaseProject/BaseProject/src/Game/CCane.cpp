#include "CCane.h"
#include "CCollisionManager.h"

// �R���X�g���N
CCane::CCane()
{
	mpCane = CResourceManager::Get<CModel>("Cane");
}

// �f�X�g���N�^
CCane::~CCane()
{
}

// �X�V
void CCane::Update()
{

}

// �`��
void CCane::Render()
{
	if (mpAttachMtx == nullptr)
	{
		mpCane->Render(Matrix());
	}
	else
	{
		CMatrix sm;
		sm.Scale(10000.0f, 10000.0f, 10000.0f);
		mpCane->Render(sm * (*mpAttachMtx));
	}
}