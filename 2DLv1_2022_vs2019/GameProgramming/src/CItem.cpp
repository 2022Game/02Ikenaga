#include "CItem.h"
#include"CItem11.h"
#include "CApplication.h"
#include <iostream>
#include <thread>
#include <chrono>
#define TEXCOORD 36,83,263,221     //時計
#define TEXCOORD2 10,10,10,20
#define TEXCOORD3 36,83,263,221

void CItem::Collision()
{
	CApplication::CharacterManager()->Collision(this);
}

void CItem::Collision(CCharacter* m, CCharacter* o)
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
			    /*int mTime = CGame::mTime;
				if (mTime > 700)
				{
					mEnabled = false;
					Texture(Texture(), TEXCOORD);
					CApplication::CharacterManager()->Add(
						new CItem11(X(),
							Y(),
							TIPSIZE, TIPSIZE, CApplication::Texture6()));
				}*/
				//mTime--;
			}
		}
		break;
	//default:
	//	if (CRectangle::Collision(o))
	//	{
	//		//mState = EState::ESTOP;
	//		mEnabled = false;
	//	}
	}
}
void CItem::Update()
{
	Texture(Texture(), TEXCOORD);
	const int PITCH = 32;//画像を切り替える間隔
	int mTime = CGame::mTime;
	//X(X() + mVx);
	if ((int)X() % PITCH < PITCH / 2)
	{
		if (mTime > 1900) //左へ移動
		{
			//左向き１を設定
			Texture(Texture(), TEXCOORD2);
			if (mTime > 1910)
			{
				Texture(Texture(), TEXCOORD);
			}
			if (mTime > 1920)
			{
				Texture(Texture(), TEXCOORD2);
			}
			if (mTime > 1930)
			{
				Texture(Texture(), TEXCOORD);
			}
			if (mTime > 1940)
			{
				Texture(Texture(), TEXCOORD2);
			}
			if (mTime > 1950)
			{
				Texture(Texture(), TEXCOORD);
			}
			if (mTime > 1960)
			{
				Texture(Texture(), TEXCOORD2);
			}
			if (mTime > 1970)
			{
				Texture(Texture(), TEXCOORD);
			}
			if (mTime > 1980)
			{
				Texture(Texture(), TEXCOORD2);
			}
			if (mTime > 1985)
			{
				Texture(Texture(), TEXCOORD);
			}
			if (mTime > 1990)
			{
				Texture(Texture(), TEXCOORD2);
			}
			if (mTime > 1995)
			{
				Texture(Texture(), TEXCOORD);
			}
			if (mTime > 2000)
			{
				mEnabled = false;
			}
		}
		else
		{
			//通常の画像を設定
			//Texture(Texture(), TEXCOORD);
		}
	}
	else
	{
		if (mTime > 900) //左へ移動
		{
			//左向き2を設定
			//mEnabled = false;
			Texture(Texture(), TEXCOORD2);
			if (mTime > 910)
			{
				Texture(Texture(), TEXCOORD);
			}
			if (mTime > 920)
			{
				Texture(Texture(), TEXCOORD2);
			}
			if (mTime > 930)
			{
				Texture(Texture(), TEXCOORD);
			}if (mTime > 940)
			{
				Texture(Texture(), TEXCOORD2);
			}
			if (mTime > 950)
			{
				Texture(Texture(), TEXCOORD);
			}
			if (mTime > 960)
			{
				Texture(Texture(), TEXCOORD2);
			}
			if (mTime > 970)
			{
				Texture(Texture(), TEXCOORD);
			}
			if (mTime > 980)
			{
				Texture(Texture(), TEXCOORD2);
			}
			if (mTime > 985)
			{
				Texture(Texture(), TEXCOORD);
			}
			if (mTime > 990)
			{
				Texture(Texture(), TEXCOORD2);
			}
			if (mTime > 995)
			{
				Texture(Texture(), TEXCOORD);
			}
			if (mTime > 1000)
			{
				mEnabled = false;
			}
			else
			{
				//2番目の画像を設定
				if (mTime > 0)
				{
					//Texture(Texture(), TEXCOORD);
				}
			}
		}
	}
}

CItem::CItem(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w, h);
	Texture( pt, TEXCOORD);
	mTag = ETag::EITEM;
}