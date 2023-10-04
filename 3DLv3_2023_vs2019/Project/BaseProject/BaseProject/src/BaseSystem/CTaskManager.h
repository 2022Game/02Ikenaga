#pragma once
#include "CTask.h"

/// <summary>
/// タスク管理クラス
/// </summary>
class CTaskManager
{
public:
	/// <summary>
	/// インスタンスを取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static CTaskManager* Instance();
	// インスタンスを破棄
	static void ClearInstance();

	/// <summary>
	/// 指定したタスクをリストに追加
	/// </summary>
	/// <param name="task">追加するタスク</param>
	void Add(CTask* task);
	/// <summary>
	/// 指定したタスクをリストから取り除く
	/// </summary>
	/// <param name="task">取り除くタスク</param>
	void Remove(CTask* task);

	// 削除フラグが立っているタスクを全て削除
	void Delete();
	/// <summary>
	/// 指定したシーンに所属するタスクを全て削除
	/// </summary>
	/// <param name="scene">削除するシーンの種類</param>
	void DeleteInScene(EScene scene);

	// 更新
	void Update();
	// 描画
	void Render();

	void Collision();

private:
	// コンストラクタ
	CTaskManager();
	// デストラクタ
	~CTaskManager();

	// タスクマネージャのインスタンス
	static CTaskManager* mpInstance;

	// 初期化
	void Initialize();

	CTask* mpHead;	// 先頭タスク
	CTask* mpTail;	// 最終タスク
};
