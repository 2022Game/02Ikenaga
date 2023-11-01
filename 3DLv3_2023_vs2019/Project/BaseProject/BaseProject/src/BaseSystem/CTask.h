#pragma once
#include "TaskPriority.h"
#include "TaskPauseType.h"
#include "SceneType.h"
class CTaskManager;

//タスククラス
class CTask
{
	friend CTaskManager;
public:
	//衝突処理
	virtual void Collision() {}

	//コンストラクタ
	CTask(ETaskPriority prio = ETaskPriority::eDefault, bool dontDelete = false);
	//デストラクタ
	virtual ~CTask();
	//更新
	virtual void Update();
	//描画
	virtual void Render();

	//優先度を設定
	void SetPriority(ETaskPriority prio);
	//ポーズの種類を設定
	void SetPauseType(ETaskPauseType type);
	//ポーズの種類を取得
	ETaskPauseType GetPauseType() const;
	//所属するシーンを設定
	void SetSceneType(EScene scene);
	//所属するシーンを取得
	EScene GetSceneType() const;

	//タスクを削除
	void Kill();
	//削除フラグ取得
	bool IsKill() const;

private:
	CTask* mpNext;		//次のポインタ
	CTask* mpPrev;		//前のポインタ
	ETaskPriority mPriority;	//優先度
	bool mEnabled;		//有効フラグ
	ETaskPauseType mPauseType;//ポーズの種類
	EScene mSceneType;	//所属するシーンの種類
};
