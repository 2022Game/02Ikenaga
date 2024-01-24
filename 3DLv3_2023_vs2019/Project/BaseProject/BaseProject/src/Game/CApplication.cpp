#include "CApplication.h"
//OpenGL
#include "glut.h"
#include "CTaskManager.h"
#include "CCollisionManager.h"
#include "CSoundManager.h"
#include "CSceneManager.h"
#include "CGamePause.h"
#include "CGameScene.h"

CApplication::~CApplication()
{
}

void CApplication::Start()
{
	new CGamePause();
	CSceneManager::Instance()->LoadScene(EScene::eBootMenu);
	//mpGameScene = new CGameScene();
}

void CApplication::End()
{
	CSceneManager::ClearInstance();
	CTaskManager::ClearInstance();
	CCollisionManager::ClearInstance();
	CResourceManager::ClearInstance();
	CSoundManager::ClearInstance();
}

void CApplication::Update()
{
	CTaskManager::Instance()->Delete();
	// タスクの更新処理
	CTaskManager::Instance()->Update();
	// 衝突処理
	CCollisionManager::Instance()->CollisionAll();

	// サウンドの更新
	CSoundManager::Instance()->Update();

	// シーンの更新
	CSceneManager::Instance()->Update();

	// タスクの描画処理
	CTaskManager::Instance()->Render();
	// コライダの描画
	CCollisionManager::Instance()->Render();

	/*switch (mState)
	{
	case EState::EPLAY:
		mpGameScene->Update();
		if (mpGameScene->IsClear())
		{
			mState = EState::ECLEAR;
		}
		if (mpGameScene->IsOver())
		{
			mState = EState::EOVER;
		}
		break;
	case EState::ECLEAR:
		mpGameScene->Clear();
		break;
	case EState::EOVER:
		mpGameScene->Over();
		break;
	}*/
}
