#pragma once
#include "CSceneBase.h"
class CPlayer;
class CSound;

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

	// シーンの更新処理
	void Update();

private:
	// プレイヤー
	CPlayer* mpPlayer;
};
