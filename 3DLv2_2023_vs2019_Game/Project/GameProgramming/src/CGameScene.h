#pragma once
#include "CSceneBase.h"

//ゲームシーン
class CGameScene :public CSceneBase
{
public:

	//コンストラクタ
	CGameScene();
	//デストラクタ
	~CGameScene();

	//シーン読み込み
	void Load();
	void Update();
};
