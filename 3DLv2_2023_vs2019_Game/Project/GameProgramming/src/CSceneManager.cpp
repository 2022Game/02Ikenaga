#include "CSceneManager.h"
#include "CTitleScene.h"
#include "CGameScene.h"

CSceneManager* CSceneManager::mpInstance = nullptr;

//インスタンスを取得
CSceneManager* CSceneManager::Instance()
{
	//インスタンスが存在しない場合は新しく生成する
	if (mpInstance == nullptr)
	{
		mpInstance = new  CSceneManager();
	}
	return mpInstance;
}

//インスタンスを破棄
void  CSceneManager::ClearInstance()
{
	delete mpInstance;
	mpInstance = nullptr;
}

//コンストラクタ
CSceneManager::CSceneManager()
	:mpScene(nullptr)
{
}

//デストラクタ
CSceneManager::~CSceneManager()
{
	//読み込んでいるシーンがあれば、破棄する
	UnloadScene();
}

//シーン読み込み
void CSceneManager::LoadScene(EScene scene)
{
	//既にシーンが読み込まれていたら、
	//そのシーンを破棄する
	UnloadScene();

	//読み込むシーンの種類に合わせて
	//生成するシーンのクラスを変更する
	switch (scene)
	{
	//タイトルシーン
	case EScene::eTitle: mpScene = new CTitleScene();   break;
	//ゲームシーン
	case EScene::eGame: mpScene = new CGameScene();   break;
	}

	//シーンを新しく生成したら読み込み
	if (mpScene != nullptr)
	{
		mpScene->Load();
	}
}

//読み込んでいるシーンを破棄する
void CSceneManager::UnloadScene()
{
	delete mpScene;
	mpScene = nullptr;
}

void CSceneManager::Update()
{
	if (mpScene == nullptr) return;
	mpScene->Update();
}