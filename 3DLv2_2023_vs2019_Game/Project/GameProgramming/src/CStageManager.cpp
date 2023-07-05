#include "CStageManager.h"
#include "CStage1.h"
#include "CStage2.h"

CStageManager* CStageManager::mpInstance = nullptr;

//インスタンスを取得
CStageManager* CStageManager::Instance()
{
	if (mpInstance == nullptr)
	{
		mpInstance = new CStageManager();
	}
	return mpInstance;
}

//コンストラクタ
CStageManager::CStageManager()
	:mpStage(nullptr)
{

}

//デストラクタ
CStageManager::~CStageManager()
{
	UnloadStage();
}

//指定されたステージを読み込み
void CStageManager::LoadStage(int no)
{
	//現在読み込んでいるステージは破棄しておく
	UnloadStage();

	switch (no)
	{
	case 0:mpStage = new CStage1(); break;
	case 1:mpStage = new CStage2(); break;
	}
}

//読み込まれているステージを破棄
void CStageManager::UnloadStage()
{
	//ステージが読み込まれてなければ、処理しない
	if (mpStage == nullptr)return;

	//ステージ破棄
	delete mpStage;
	mpStage = nullptr;
}