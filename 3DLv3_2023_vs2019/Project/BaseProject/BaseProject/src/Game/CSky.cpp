#include "CSky.h"

// �R���X�g���N
CSky::CSky()
	: CObjectBase(ETag::eSky, ETaskPriority::eBackground)
{
	mpModel = CResourceManager::Get<CModel>("Sky");
}

// �f�X�g���N�^
CSky::~CSky()
{
}

// �X�V
void CSky::Update()
{
	Rotate(0.0f, 0.01f, 0.0f);
}

// �`��
void CSky::Render()
{
	mpModel->Render(Matrix());
}