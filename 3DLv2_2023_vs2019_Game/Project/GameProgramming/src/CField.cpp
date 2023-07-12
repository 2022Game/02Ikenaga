#include "CField.h"
#include "CTaskManager.h"

//コンストラクタ
CField::CField()
{
	//ポーズの種類を設定
	SetPauseType(TaskPauseType::eGame);
	//タスクの優先度設定
	mPriority = (int)TaskPriority::eBackground;
	//タスクマネージャーに自信追加
	CTaskManager::Instance()->Add(this);

	mModel = new CModel();
	mModel->Load("res\\sky.obj", "res\\sky.mtl");
	mColliderMesh.Set(nullptr, nullptr, mModel);
}

//デストラクタ
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