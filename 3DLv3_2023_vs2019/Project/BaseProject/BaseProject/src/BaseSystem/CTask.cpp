#include "CTask.h"
#include "CTaskManager.h"
#include "CSceneManager.h"

//コンストラクタ
CTask::CTask(ETaskPriority prio, bool dontDelete)
	: mpNext(nullptr)
	, mpPrev(nullptr)
	, mPriority(prio)
	, mEnabled(true)
	, mPauseType(ETaskPauseType::eDefault)
{
	//シーン遷移で破棄しないタスクかどうかで
	//所属するシーンのタイプを切り替える
	mSceneType = dontDelete
		? EScene::eNone
		: CSceneManager::Instance()->GetCurrentScene();

	//タスクを生成した時に
	//タスクマネージャーのタスクリストに追加
	CTaskManager::Instance()->Add(this);
}

//デストラクタ
CTask::~CTask()
{
	//タスクマネージャーのタスクリストから取り除く
	CTaskManager::Instance()->Remove(this);
}

//更新
void CTask::Update()
{
}

//描画
void CTask::Render()
{
}

//優先度を設定
void CTask::SetPriority(ETaskPriority prio)
{
	//既に設定されている優先度であれば処理しない
	if (mPriority == prio) return;

	//一度タスクリストから取り除き、再度追加することで、
	//新しい優先度の場所にタスクを差し込む
	mPriority = prio;
	CTaskManager::Instance()->Remove(this);
	CTaskManager::Instance()->Add(this);
}

//ポーズの種類を設定
void CTask::SetPauseType(ETaskPauseType type)
{
	mPauseType = type;
}

//ポーズの種類を取得
ETaskPauseType CTask::GetPauseType() const
{
	return mPauseType;
}

//所属するシーンを設定
void CTask::SetSceneType(EScene scene)
{
	mSceneType = scene;
}

//所属するシーンを取得
EScene CTask::GetSceneType() const
{
	return mSceneType;
}

//タスクを削除
void CTask::Kill()
{
	mEnabled = false;
}

//削除フラグ取得
bool CTask::IsKill() const
{
	return !mEnabled;
}