#pragma once
#include "CEnemy.h"
#include "CPlayer.h"
#include "CSound.h"
#include "CModel.h"
#include "CModelX.h"

class CApplication
{
private:
	CPlayer* mpPlayer;

	CSound mSoundBgm;	        //BGM
	CSound mSoundOver;	        //ゲームオーバー
public:
	~CApplication();

	//最初に一度だけ実行するプログラム
	void Start();
	//繰り返し実行するプログラム
	void Update();
};