#pragma once
#include "CTexture.h"
#include "CInput.h"
#include "CSound.h"
#include"CVector.h"
#include"CTaskManager.h"
#include"CUi.h"
#include "CCharacter3.h"

class CApplication
{
private:
	//int mSTcount;
	static CUi* spUi; //UIクラスのポインタ
	//モデルからコライダを生成
	//CColliderMesh mColliderMesh;
	//モデルビューの逆行列
	static CMatrix mModelViewInverse;
	//CPlayer mPlayer;
	//CCharacter3 mCharacter;
	//CModel* mpBackGround;//背景モデル
	CSound mSoundBgm;	//BGM
	CSound mSoundOver;	//ゲームオーバー
	static CTexture mTexture;
	CInput mInput;
public:
	~CApplication();
	static CUi* Ui();  //UIクラスのインスタンスを取得
	//モデルビュー行列の取得
	static const CMatrix& ModelViewInverse();
	//最初に一度だけ実行するプログラム
	void Start();
	//繰り返し実行するプログラム
	void Update();
};