#include "CSceneBase.h"
#include "CTaskManager.h"

//�R���X�g���N�^
CSceneBase::CSceneBase(EScene scene)
	:mSceneType(scene)
{

}

//�f�X�g���N�^
CSceneBase::~CSceneBase()
{
	CTaskManager::Instance()->DeleteInScene(mSceneType);
}

EScene CSceneBase::GetSceneType() const
{
	return mSceneType;
}

void CSceneBase::AddTask(CTask* task)
{
	if (task == nullptr) return;
	task->SetSceneType(mSceneType);
}