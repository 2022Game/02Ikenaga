#include "CApplication.h"
//OpenGL
#include"glut.h"
#include"CVector.h"
#include"CBillBoard.h"
#include "CResourceManager.h"
#include "CStageManager.h"
#include "CSceneManager.h"
#include "CCollisionManager.h"
#include"CClear.h"
#include"CPlayer.h"

//#define TEXTYA "AvatarHeadMale_Color_2K.jpeg"

#define SOUND_BGM "res\\mario.wav" //BGM音声ファイル
#define SOUND_OVER "res\\mdai.wav" //ゲームオーバー音声ファイル

CApplication::~CApplication()
{
	delete spUi;  //インスタンスUiの削除
}

CUi* CApplication::spUi = nullptr;

CUi* CApplication::Ui() 
{
	return spUi;  //インスタンスのポインタを返す
}

CMatrix  CApplication::mModelViewInverse;

const CMatrix& CApplication::ModelViewInverse()
{
	return mModelViewInverse;
}

void CApplication::Start()
{
	CSceneManager::Instance()->LoadScene(EScene::eTitle);
	spUi = new CUi();  //UIクラスの生成
	spUi->STCOUNT(CClear::STcount());
	//spUi->Hp(CPlayer::Hp());
}

void CApplication::Update()
{
	if (mInput.PullKey('1'))
	{
		CStageManager::Instance()->LoadStage(0);
	}
	else if (mInput.PullKey('2'))
	{
		CStageManager::Instance()->LoadStage(1);
	}
	else if (mInput.PullKey('3'))
	{
		CStageManager::Instance()->LoadStage(2);
	}
	else if (mInput.PullKey('4'))
	{
		CStageManager::Instance()->LoadStage(3);
	}
	else if (mInput.PullKey('5'))
	{
		CStageManager::Instance()->LoadStage(4);
	}
	spUi->STCOUNT(CClear::STcount());
	spUi->Hp(CPlayer::Hp());
	//タスクマネージャの更新
	CTaskManager::Instance()->Update();
	CTaskManager::Instance()->Collision();
	//視点の設定
	//gluLookAt(視点X,視点Y,視点Z,中心X,中心Y,中心Z,上向X,上向Y,上向Z)
	//gluLookAt(mEye.X(), mEye.Y(), mEye.Z(), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	CTransform trans;  //変換行列インスタンスの作成
	trans.Position(CVector(1.0f, 4.0f, 2.0f));//位置の設定
	trans.Rotation(CVector(-10.0f, -20.0f, -30.0f));  //回転の設定
	trans.Scale(CVector(1.0f, 1.0f, 1.0f));//拡大縮小の設定
	trans.Update(); //行列の更新
	//mPlayer.Update();
	CSceneManager::Instance()->Update();
	
	//モデルビュー行列の取得
	glGetFloatv(GL_MODELVIEW_MATRIX, mModelViewInverse.M());
	//逆行列の取得dd
	mModelViewInverse = mModelViewInverse.Transpose();
	mModelViewInverse.M(0, 3, 0);
	mModelViewInverse.M(1, 3, 0);
	mModelViewInverse.M(2, 3, 0);
	//タスクリストの削除
	CTaskManager::Instance()->Delete();
	//タスクマネージャの描画
	CTaskManager::Instance()->Render();
	CCollisionManager::Instance()->Render();
	spUi->Render();  //UIの描画
}
