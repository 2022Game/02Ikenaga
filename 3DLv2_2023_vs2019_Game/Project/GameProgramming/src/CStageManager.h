#pragma once
#include "CStageBase.h"

class CStageManager
{
public:
	//インスタンスを取得
	static CStageManager* Instance();

	//コンストラクタ
	CStageManager();
	//デストラクタ
	~CStageManager();

	//指定されたステージを読み込み
	void LoadStage(int no);
	//読み込まれているステージを破棄
	void UnloadStage();

private:
	//CStageManagerのインスタンス
	static CStageManager* mpInstance;
	//現在読み込んでいるステージ
	CStageBase* mpStage;
};