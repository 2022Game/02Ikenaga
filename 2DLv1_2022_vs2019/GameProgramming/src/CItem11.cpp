#include "CItem11.h"
#include "CApplication.h"
#include <iostream>
#include <thread>
#include <chrono>
#define TEXCOORD 36,83,263,221     //時計
#define TEXCOORD2 36,83,263,221    

void CItem11::Collision()
{
	CApplication::CharacterManager()->Collision(this);
}

void CItem11::Collision(CCharacter* m, CCharacter* o)
{
	//めり込み調整変数を宣言する
	float x, y;
	switch (o->Tag())
	{
		//case ETag::EITEM:
			//敵の衝突判定を実行
		o->Collision(o, m);
		X(X() + x);
		Y(Y() + y);
		if (x != 0.0f)
		{
			//X軸速度を0にする
			mVx = 0.0f;
			if (x > 0.0f)
			{
				mState = EState::EMOVE;
			}
			else
			{	//ジャンプでなければ泣く
				mState = EState::ECRY;
			}
		}
	case ETag::EPLAYER:
		if (CRectangle::Collision(o))
		{
			if (o->Tag() == ETag::EPLAYER)
			{
				mEnabled = false;
				//mTime--;
			}
		}
		break;
	}
}
void CItem11::Update()
{
	//通常の画像を設定
	Texture(Texture(), TEXCOORD);
}

CItem11::CItem11(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w, h);
	Texture(pt, TEXCOORD);
	mTag = ETag::EITEM;
	/*int mTime = CGame::mTime;
	if (mTime > 500) 
	{
		mEnabled = false;
	}*/
}