#pragma once
#include "CSceneBase.h"
class CPlayer;
class CSlime;
class CSlime2;

// タイトルシーン
class CGameScene :public CSceneBase
{
public:
	// コンストラクタ
	CGameScene();
	// デストラクタ
	~CGameScene();
	// シーン読み込み
	void Load();

	// ゲームクリア判定
	bool IsClear();
	// ゲームクリア処理
	void Clear();
	// ゲームオーバー判定
	bool IsOver();
	// ゲームオーバー処理
	void Over();
	// シーンの更新処理
	void Update();

private:
	// 復活時間
	int RevivalTime;
	int RevivalTime2;
	CPlayer* mpPlayer;
	CSlime* mpSlime;
	CSlime2* mpSlime2;
};
