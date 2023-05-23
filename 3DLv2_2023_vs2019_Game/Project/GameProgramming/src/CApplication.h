#pragma once
#include "CRectangle.h"
#include "CTexture.h"
#include "CCharacter.h"
#include "CBullet.h"
#include "CEnemy.h"
#include "CPlayer.h"
#include "CInput.h"
#include "CFont.h"
#include "CMiss.h"
#include <vector>
#include "CCharacterManager.h"
#include "CGame.h"
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

class CApplication
{
private:
	static CUi* spUi; //UIクラスのポインタ
	//モデルからコライダを生成
	CColliderMesh mColliderMesh;
	//三角コライダの作成
	CColliderTriangle mColliderTriangle;
	CColliderTriangle mColliderTriangle2;
	//モデルビューの逆行列
	static CMatrix mModelViewInverse;
	CModel mStage;
	CModel mKokki;
	//C5モデル
	CModel mModelC5;
	//static CTaskManager mTaskManager;
	CPlayer mPlayer;
	//CCharacter3 mCharacter;
	CModel mBackGround;//背景モデル
	//モデルクラスのインスタンス作成
	CModel mModel;
	//CModel mModel2;
	CTriangle* mTriangle;
	CVector mEye;
	CSound mSoundBgm;	//BGM
	CSound mSoundOver;	//ゲームオーバー
	CGame* mpGame;
	static CCharacterManager mCharacterManager;
	enum class EState
	{
		ESTART,	//ゲーム開始
		EPLAY,	//ゲーム中
		ECLEAR,	//ゲームクリア
		EOVER,	//ゲームオーバー
	};
	EState mState;
//	CCharacter mRectangle;
	//CPlayer* mpPlayer;
	static CTexture mTexture;
	//static CTexture mTexture1;
	CEnemy* mpEnemy;
//	CBullet* mpBullet;
	CInput mInput;
	CFont mFont;
	CMiss* mpMiss;
	//CCharacterのポインタの可変長配列
//	std::vector<CCharacter*> mCharacters;
public:
	~CApplication();
	static CUi* Ui();  //UIクラスのインスタンスを取得
	//モデルビュー行列の取得
	static const CMatrix& ModelViewInverse();
	//static CTaskManager* TaskManager();
	static CCharacterManager* CharacterManager();
	static CTexture* Texture();
	//static CTexture* Texture1();
	//最初に一度だけ実行するプログラム
	void Start();
	//繰り返し実行するプログラム
	void Update();
};