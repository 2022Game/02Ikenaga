#pragma once
#include "CSceneBase.h"
class CSlime;
class CSlime2;

//タイトルシーン
class CGameScene :public CSceneBase
{
public:
	//コンストラクタ
	CGameScene();
	//デストラクタ
	~CGameScene();
	//シーン読み込み
	void Load();
	//シーンの更新処理
	void Update();

private:
	// 復活時間
	int RevivalTime;
	CSlime* mpSlime;
	CSlime2* mpSlime2;
};
