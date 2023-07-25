#include "CTaskManager.h"

//タスクマネージャのインスタンス
CTaskManager* CTaskManager::mpInstance = nullptr;

//インスタンスの取得
CTaskManager* CTaskManager::Instance()
{
	//インスタンスが無ければ
	if (mpInstance == nullptr)
	{	//インスタンスを生成する
		mpInstance = new CTaskManager();
	}
	return mpInstance;
}

//デフォルトコンストラクタ
CTaskManager::CTaskManager()
	: mPauseBit(0)
{
	mHead.mpNext = &mTail;
	mTail.mpPrev = &mHead;
}

void CTaskManager::Delete() {
	//先頭から最後まで繰り返し
	CTask* task = mHead.mpNext;
	while (task->mpNext) {
		CTask* del = task;
		//次へ
		task = task->mpNext;
		//mEnabledがfalseなら削除
		if (del->mEnabled == false) {
			delete del;
		}
	}
}

//シーン内のタスクを全て削除
void CTaskManager::DeleteInScene(EScene scene)
{
	//先頭から最後まで繰り返し
	CTask* task = mHead.mpNext;
	while (task->mpNext)
	{
		CTask* del = task;
		//次へ
		task = task->mpNext;
		//所属するシーンが一致したら削除
		if (del->GetSceneType() == scene)
		{
			del->mEnabled = false;
		}
	}
}

void CTaskManager::Remove(CTask* task)
{
	//タスクの前の次を、タスクの次にする
	task->mpPrev->mpNext = task->mpNext;
	//タスクの次の前を、タスクの前にする
	task->mpNext->mpPrev = task->mpPrev;
}
CTaskManager::~CTaskManager() {
}
//リストに追加
//Add(タスクのポインタ)
void CTaskManager::Add(CTask* addTask)
{
	//mHeadの次から検索
	CTask* task = mHead.mpNext;

	//優先度の大きい順に挿入する
	//挿入位置の検索（優先度が同じか大きくなった前）
	//mPriority>=0のこと
	while (task != &mTail && addTask->mPriority < task->mPriority)
	{
		task = task->mpNext; //次へ
	}

	//addTaskの次をtask
	addTask->mpNext = task;
	//addTaskの前をtaskの前に
	addTask->mpPrev = task->mpPrev;
	//addTaskの前の次をaddTaskに
	addTask->mpPrev->mpNext = addTask;
	//taskの前をaddTaskに
	task->mpPrev = addTask;
}

//更新
void CTaskManager::Update() {
	//先頭から最後まで繰り返し
	CTask* task = mHead.mpNext;
	while (task->mpNext) {
		//ポーズ中のタスクでなければ
		TaskPauseType pause = task->GetPauseType();
		if (pause == TaskPauseType::eNone || (mPauseBit & (int)pause) == 0)
		{
			//更新処理を呼ぶ
			task->Update();
		}
		//次へ
		task = task->mpNext;
	}
}

//描画
void CTaskManager::Render() {
	//最後から先頭まで繰り返し
	CTask* task = mTail.mpPrev;
	while (task->mpPrev) {
		//非表示状態でなければ
		if (!task->IsHidden())
		{
			//描画処理を呼ぶ
			task->Render();
		}
		//次へ
		task = task->mpPrev;
	}
}

//衝突処理
void CTaskManager::Collision()
{
	//先頭から最後まで繰り返し
	CTask* task = mHead.mpNext;
	while (task->mpNext) {
		if (!task->IsKill())
		{
			//衝突処理を呼ぶ
			task->Collision();
		}
		//次へ
		task = task->mpNext;
	}
}

//ポーズを設定
void CTaskManager::Pause(int pauseBit)
{
	mPauseBit |= pauseBit;
}

//ポーズを解除する
void CTaskManager::UnPause(int pauseBit)
{
	mPauseBit &= ~pauseBit;
}

//ポーズ中かどうか
bool CTaskManager::IsPaused(int pauseBit) const
{
	if (pauseBit == 0) pauseBit = ~pauseBit;
	return (mPauseBit & pauseBit) != 0;
}