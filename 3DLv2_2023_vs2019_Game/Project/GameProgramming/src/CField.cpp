#include "CField.h"
#include "CTaskManager.h"

//�R���X�g���N�^
CField::CField()
{
	//�|�[�Y�̎�ނ�ݒ�
	SetPauseType(TaskPauseType::eGame);
	//�^�X�N�̗D��x�ݒ�
	mPriority = (int)TaskPriority::eBackground;
	//�^�X�N�}�l�[�W���[�Ɏ��M�ǉ�
	CTaskManager::Instance()->Add(this);

	mModel = new CModel();
	mModel->Load("res\\sky.obj", "res\\sky.mtl");
	mColliderMesh.Set(nullptr, nullptr, mModel);
}

//�f�X�g���N�^
CField::~CField()
{
	CTaskManager::Instance()->Remove(this);

	delete mModel;
	mModel = nullptr;
}

void CField::Update()
{

}

void CField::Render()
{
	if (mModel == nullptr) return;
	mModel->Render();
}