#include "CTaskManager.h"
#include "CCamera.h"

//タスクマネージャのインスタンス
CTaskManager* CTaskManager::mpInstance = nullptr;

// インスタンスを取得
CTaskManager* CTaskManager::Instance()
{
	// インスタンスが無ければ、新しく生成する
	if (mpInstance == nullptr)
	{
		mpInstance = new CTaskManager();
		mpInstance->Initialize();
	}
	return mpInstance;
}

// インスタンスの破棄
void CTaskManager::ClearInstance()
{
	delete mpInstance;
	mpInstance = nullptr;
}

// コンストラクタ
CTaskManager::CTaskManager()
	: mpHead(nullptr)
	, mpTail(nullptr)
{
}

// デストラクタ
CTaskManager::~CTaskManager()
{
}

// 初期化
void CTaskManager::Initialize()
{
	mpHead = new CTask();
	mpTail = new CTask();

	mpHead->mpNext = mpTail;
	mpTail->mpPrev = mpHead;
}

// 指定したタスクをリストに追加
void CTaskManager::Add(CTask* task)
{
	if (mpHead == nullptr) return;
	if (mpHead->mpNext == nullptr) return;

	// mHeadの次から検索
	CTask* next = mpHead->mpNext;

	// 優先度の数値が小さい順に並べる
	// 挿入位置の検索
	// （追加するタスクの優先度より大きいタスクの前に挿入）
	while (next != mpTail && task->mPriority >= next->mPriority)
	{
		next = next->mpNext;	// 次へ
	}

	// 次タスクの前のタスクを覚えておく
	CTask* prev = next->mpPrev;

	// [prev]→[task]→[next] の順に各タスクの次のタスクを繋ぐ
	prev->mpNext = task;	// prev → task を繋ぐ
	task->mpNext = next;	// task → next を繋ぐ

	// [prev]←[task]←[next] の順に各タスクの前のタスクを繋ぐ
	task->mpPrev = prev;	// prev ← task を繋ぐ
	next->mpPrev = task;	// task ← next を繋ぐ
}

// 指定したタスクをリストから取り除く
void CTaskManager::Remove(CTask* task)
{
	// 取り除くタスクの前後のタスクを繋ぐ
	CTask* prev = task->mpPrev;
	CTask* next = task->mpNext;

	// [prev]→[next] を繋ぐ
	prev->mpNext = next;
	// [prev]←[next] を繋ぐ
	next->mpPrev = prev;

	// 取り除くタスクの前後のタスクを初期化
	task->mpPrev = nullptr;
	task->mpNext = nullptr;
}

// 削除フラグが立っているタスクを全て削除
void CTaskManager::Delete()
{
	// 先頭から最後まで繰り返し
	CTask* task = mpHead->mpNext;
	while (task != mpTail)
	{
		// 削除タスクを記憶しておく
		CTask* del = task;
		// 次へ
		task = task->mpNext;

		// 削除フラグが立っていたら、削除
		if (del->IsKill())
		{
			delete del;
		}
	}
}

// 指定したシーンに所属するタスクを全て削除
void CTaskManager::DeleteInScene(EScene scene)
{
	// 先頭から最後まで繰り返し
	CTask* task = mpHead->mpNext;
	while (task != mpTail)
	{
		// 所属するシーンが一致したら、
		// 削除フラグを立てる
		if (task->GetSceneType() == scene)
		{
			task->Kill();
		}
		// 次へ
		task = task->mpNext;
	}
}

// 更新
void CTaskManager::Update()
{
	// 先頭から最後まで繰り返し
	CTask* task = mpHead->mpNext;
	while (task != mpTail)
	{
		// タスクを更新
		task->Update();
		// 次へ
		task = task->mpNext;
	}
}

// 描画
void CTaskManager::Render()
{
	// 先頭から最後まで繰り返し
	CTask* task = mpHead->mpNext;

	// 3D関連の描画
	// メインカメラを反映
	CCamera::MainCamera()->Apply();
	// 先頭から2D関連のタスクまで描画する
	while (task != mpTail && task->mPriority < ETaskPriority::Start2d)
	{
		// タスクを描画
		task->Render();
		task = task->mpNext;
	}

	// 2D関連の描画
	// 2D描画用のカメラに切り替える
	CCamera::Start2DCamera();
	while (task != mpTail)
	{
		// タスクを描画
		task->Render();
		task = task->mpNext;
	}
	// 3D描画用のカメラへ戻す
	CCamera::End2DCamera();
}

//衝突処理
void CTaskManager::Collision()
{
	//先頭から最後まで繰り返し
	CTask* task = mpHead->mpNext;
	while (task->mpNext)
	{
		//衝突処理を呼ぶ
		task->Collision();
		//次へ
		task = task->mpNext;
	}
}
