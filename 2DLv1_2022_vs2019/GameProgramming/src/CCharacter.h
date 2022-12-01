#pragma once
#include "CRectangle.h"
#include "CTexture.h"
#include <stdio.h>

#define VELOCITY 2.0f	//移動速度
#define VELOCITY2  2.5f;
#define SOKUDO   1.0f

class CCharacter : public CRectangle
{
public:
	enum class ETag
	{
		EZERO,		//初期値
		EBULLET,	//弾
		EENEMY,		//敵
		EPLAYER,	//プレイヤー
		ETURN,		//折り返し
		EBLOCK,	    //ブロック
		EBLOCK2,    //通れるブロック
		EBLOCK3,    //動くブロック
		EBLOCK4,
		EGOAL,      //ゴール
		EITEM,      //アイテム
		EITEM2,
		EITEM3,
		EATTACK,    //攻撃
	};
	enum class EState	//状態
	{
		EMOVE,	//移動
		ESTOP,	//停止
		EJUMP,	//ジャンプ
		ECRY, //泣く
		EHIT, //当たってる
		ENOTHIT, //あたらない
		EATTACK, //攻撃
		EANNIHILATION,  //消滅
		EUP,
		EDOWN,
		EJUMPUP,
	};
protected:
	float mVx;	//X軸速度
	float mVy;	//Y軸速度
	bool mEnabled;  //有効フラグ
	//bool mDrop; //ドロップ
	ETag mTag;
	EState mState;
private:
	CTexture *mpTexture;
	int mLeft, mRight, mBottom, mTop;
public:
	EState State();	//状態を取得する
	bool Enabled();
	ETag Tag();
	//衝突処理２
	virtual void Collision() {};
	//衝突処理４
	//Collision(自分のポインタ, 衝突相手のポインタ)
	virtual void Collision(CCharacter* m, CCharacter* o) {};
	virtual void Update() = 0;
	CCharacter();
	CTexture* Texture();
	void Texture(CTexture *pTexture, int left, int right, int bottom, int top);
	virtual void Render();
	void Move();
};