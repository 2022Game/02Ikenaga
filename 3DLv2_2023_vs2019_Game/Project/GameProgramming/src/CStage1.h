#pragma once
#include "CStageBase.h"
#include "CPlayer.h"
#include "CUi.h"

class CStage1:public CStageBase
{
public:
	//ゲームクリア判定
	bool IsClear();
	//ゲームクリア処理
	void Clear();
	//ゲームオーバー判定
	bool IsOver();
	//ゲームオーバー処理
	void Over();
	void Start();
	void Update();
	//コンストラクタ
	CStage1();
	//デストラクタ
	~CStage1();
private:
	CPlayer* mpPlayer;
	CUi* mpUi;
};