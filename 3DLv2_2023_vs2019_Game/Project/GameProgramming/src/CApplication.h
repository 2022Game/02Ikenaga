#pragma once
#include "CRectangle.h"
#include "CTexture.h"
#include "CCharacter.h"
#include "CBullet.h"
#include "CEnemy.h"
#include"CEnemy2.h"
#include "CPlayer.h"
#include "CInput.h"
#include "CFont.h"
#include "CMiss.h"
#include <vector>
#include "CCharacterManager.h"
#include "CSound.h"
#include"CVector.h"
#include"CModel.h"
#include"CTriangle.h"
#include"CMatrix.h"
#include"CTransform.h"
#include"CCharacter3.h"
#include"CTaskManager.h"
#include"CCollisionManager.h"
#include"CColliderTriangle.h"
#include"CColliderMesh.h"
#include"CEnemy3.h"
#include"CStage.h"
#include"CKokki.h"
#include"CHATA.h"
#include"CPolice.h"
#include"CTransparent.h"
#include"CFrame.h"
#include"CClear.h"
#include"CUi.h"
#include"CStage1.h"

class CApplication
{
private:
	CStage1* mpStage1;
	static CUi* spUi; //UIクラスのポインタ
	//モデルからコライダを生成
	CColliderMesh mColliderMesh;
	//モデルビューの逆行列
	static CMatrix mModelViewInverse;
	CPlayer mPlayer;
	//CCharacter3 mCharacter;
	CModel* mpBackGround;//背景モデル
	CVector mEye;
	CSound mSoundBgm;	//BGM
	CSound mSoundOver;	//ゲームオーバー
	enum class EState
	{
		ESTART,	//ゲーム開始
		EPLAY,	//ゲーム中
		ECLEAR,	//ゲームクリア
		EOVER,	//ゲームオーバー
	};
	EState mState;
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