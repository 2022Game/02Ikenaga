#pragma once
#include "CCharacter.h"
#include "CInput.h"
#include "CSound.h"
#include "CAttack.h"
#include"CUi.h"
//#include"CGame.h"

class CPlayer2 : public CCharacter
{
public:
	static int Pulltime();
	static int Point();
	//HPを取得
	static int Hp();
	//static int Time();
	//衝突処理2
	void Collision();
	//衝突処理4
	void Collision(CCharacter* m, CCharacter* o);
	//CPlayer2(X座標,Y座標,幅,高さ,テクスチャのポインタ)
	CPlayer2(float x, float y, float w, float h, CTexture* pt);
	//更新処理
	void Update();
private:
	CSound mSoundJump;	//ジャンプSE
	static int sHp;	//HP
	static int sPoint;
	//static int sPulltime;
  // static int sTime;
	int mnothing; //なにもなし
	int mUp;//上がる
	int mdown;
	int mInvincible; //無敵カウンタ
	CInput mInput;
	//CGame *mGame;
	CUi *mpUi;
	//int mTime;
	static int mPulltime;
	float mVy;//Y軸速度
	float mVx;
};