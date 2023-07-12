#include "CSceneBase.h"
#include "CTaskManager.h"

//コンストラクタ
CSceneBase::CSceneBase(EScene scene)
	:mSceneType(scene)
{

}

//デストラクタ
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