#include "CSceneBase.h"
#include "CTaskManager.h"

//コンストラクタ
CSceneBase::CSceneBase(EScene scene)
	: mSceneType(scene)
{
}

//デストラクタ
CSceneBase::~CSceneBase()
{
	//削除するシーンに所属するタスクは全て削除
	CTaskManager::Instance()->DeleteInScene(mSceneType);
}

//シーンの種類を取得
EScene CSceneBase::GetSceneType() const
{
	return mSceneType;
}

//シーンに所属するタスクを追加
void CSceneBase::AddTask(CTask* task)
{
	if (task == nullptr) return;
	task->SetSceneType(mSceneType);
}
