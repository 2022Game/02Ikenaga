#include "CStageManager.h"
#include "CStage1.h"
#include "CStage2.h"
#include "CStage3.h"
#include "CStage4.h"
#include "CStage5.h"

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
	case 2:mpStage = new CStage3(); break;
	case 3:mpStage = new CStage4(); break;
	case 4:mpStage = new CStage5(); break;
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