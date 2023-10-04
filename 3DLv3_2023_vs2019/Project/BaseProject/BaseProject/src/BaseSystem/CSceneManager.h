#pragma once
#include "CSceneBase.h"
#include "SceneType.h"

class CSceneManager
{
public:
	/// <summary>
	/// インスタンスを取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static CSceneManager* Instance();
	// インスタンスを破棄
	static void ClearInstance();

	// シーン読み込み
	void LoadScene(EScene scene);
	// 読み込んでいるシーンを破棄
	void UnloadScene();

	/// <summary>
	/// 現在読み込んでいるシーンの種類を取得
	/// </summary>
	/// <returns>シーンの種類（eNoneの場合はシーンが読み込まれていない）</returns>
	EScene GetCurrentScene() const;

	// 読み込んでいるシーンの更新処理
	void Update();

private:
	// コンストラクタ
	CSceneManager();
	// デストラクタ
	~CSceneManager();

	// シーンマネージャのインスタンス
	static CSceneManager* mpInstance;
	// 現在読み込んでいるシーン
	CSceneBase* mpScene;
};