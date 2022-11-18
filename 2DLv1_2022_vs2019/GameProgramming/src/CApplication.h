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
#include "CGoal.h"

class CApplication
{
private:
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
	CPlayer* mpPlayer;
	static CTexture mTexture;
	static CTexture mTexture1;
	static CTexture mTexture2;
	static CTexture mTexture3;
	static CTexture mTexture4;
	static CTexture mTexture5;
	static CTexture mTexture6;
	static CTexture mTexture7;
	static CTexture mTexture8;
	CEnemy* mpEnemy;
//	CBullet* mpBullet;
	CInput mInput;
	CFont mFont;
	CFont mFont1;
	CMiss* mpMiss;
	//CCharacterのポインタの可変長配列
//	std::vector<CCharacter*> mCharacters;
public:
	static CCharacterManager* CharacterManager();
	static CTexture* Texture();
	static CTexture* Texture1();
	static CTexture* Texture2();
	static CTexture* Texture3();
	static CTexture* Texture4();
	static CTexture* Texture5();
	static CTexture* Texture6();
	static CTexture* Texture7();
	static CTexture* Texture8();
	//最初に一度だけ実行するプログラム
	void Start();
	//繰り返し実行するプログラム
	void Update();
};