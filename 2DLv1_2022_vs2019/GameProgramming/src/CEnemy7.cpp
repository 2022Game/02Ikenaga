#include"CEnemy7.h"
#include"CItem.h"
#include"CItem2.h"
#include"CItem3.h"
#include"CItem11.h"
#include"CAttack.h"
#include "CApplication.h"
#include"stdio.h"
#include "stdlib.h"
#include"time.h"

#define TEXCOORD 147, 165, 97, 79	//テクスチャマッピング
#define TEXCOORDEX 195, 213, 97, 74
#define TEXCOORDY 171,189,97,75

int CEnemy7::sNum = 0;

void CEnemy7::Num(int num)
{
	sNum = num;
}

int CEnemy7::Num()
{
	return sNum;
}

void CEnemy7::Collision()
{
	CApplication::CharacterManager()->Collision(this);
}

void CEnemy7::Collision(CCharacter* m, CCharacter* o)
{
	//めり込み調整変数を宣言する
	float x, y;
	switch (o->Tag())
	{
	case ETag::EBLOCK:
		//折り返しに当たった時
		if (CRectangle::Collision(o, &x, &y))
		{
			//めり込まない位置まで戻す
			X(X() + x);
			Y(Y() + y);
			//X軸速度を反転させる
			mVy = -mVy;
		}
		break;
	case ETag::EENEMY:
		break;
	case ETag::EPLAYER:
		if (CRectangle::Collision(o))
		{
			if (mState != EState::ECRY)
			{
				Texture(Texture(), TEXCOORDEX);
			}
			if (o->State() == EState::EJUMP)
			{
				//敵数1減算
				if (mState != EState::EANNIHILATION)
				{
					sNum--;
					//Texture(Texture(), TEXCOORDEX);
				}
				mEnabled = false;
				srand((unsigned int)time(nullptr));
				bool d_flag = false;
				int num_pro_d = rand() % 100 + 1;
				int num_pro_m = 100 - num_pro_d;
				//int num_pro_h = 100 - num_pro_d;
				//int num_pro_v = 100 - num_pro_d;
				double tmp_random = (double)rand() / RAND_MAX;
				if (tmp_random < (double)num_pro_d / 100)
				{
					d_flag = true;
				}
				else if ((rand() % 100) < 50) {
					{
						CApplication::CharacterManager()->Add(
							new CItem3(X(),
								Y(),
								TIPSIZE, TIPSIZE, CApplication::Texture6()));
					}
				}
				else if ((rand() % 100) < 25) {
					{
						CApplication::CharacterManager()->Add(
							new CItem2(X(),
								Y(),
								TIPSIZE, TIPSIZE, CApplication::Texture6()));
					}
				}
				else
				{
					if ((rand() % 100) < 25)
					{
						CApplication::CharacterManager()->Add(
							new CItem11(X(),
								Y(),
								TIPSIZE, TIPSIZE, CApplication::Texture6()));
					}
				}
				return;
			}
		}
		break;
		if (CRectangle::Collision(o))
		{
			if (o->State() == EState::EUP)
			{
				//敵数1減算
				if (mState != EState::ECRY)
				{
					sNum--;
				}
				mEnabled = false;
				srand((unsigned int)time(nullptr));
				bool d_flag = false;
				int num_pro_d = rand() % 100 + 1;
				int num_pro_m = 100 - num_pro_d;
				//int num_pro_h = 100 - num_pro_d;
				//int num_pro_v = 100 - num_pro_d;
				double tmp_random = (double)rand() / RAND_MAX;
				if (tmp_random < (double)num_pro_d / 100)
				{
					d_flag = true;
				}
				else if ((rand() % 100) < 50) {
					{
						CApplication::CharacterManager()->Add(
							new CItem3(X(),
								Y(),
								TIPSIZE, TIPSIZE, CApplication::Texture6()));
					}
				}
				else if ((rand() % 100) < 25) {
					{
						CApplication::CharacterManager()->Add(
							new CItem2(X(),
								Y(),
								TIPSIZE, TIPSIZE, CApplication::Texture6()));
					}
				}
				else
				{
					if ((rand() % 100) < 25)
					{
						CApplication::CharacterManager()->Add(
							new CItem11(X(),
								Y(),
								TIPSIZE, TIPSIZE, CApplication::Texture6()));
					}
				}
				return;
			}
		}
		break;
	case ETag::EATTACK:
		if (CRectangle::Collision(o))
		{
			if (o->State() != EState::EATTACK)
			{
				//敵数1減算
				if (mState != EState::EANNIHILATION)
				{
					//sNum--;
				}
				mEnabled = false;
				mEnabled = false;
				srand((unsigned int)time(nullptr));
				bool d_flag = false;
				int num_pro_d = rand() % 100 + 1;
				int num_pro_m = 100 - num_pro_d;
				//int num_pro_h = 100 - num_pro_d;
				//int num_pro_v = 100 - num_pro_d;
				double tmp_random = (double)rand() / RAND_MAX;
				if (tmp_random < (double)num_pro_d / 100)
				{
					d_flag = true;
				}
				else if ((rand() % 100) < 50) {
					{
						CApplication::CharacterManager()->Add(
							new CItem3(X(),
								Y(),
								TIPSIZE, TIPSIZE, CApplication::Texture6()));
					}
				}
				else if ((rand() % 100) < 25) {
					{
						CApplication::CharacterManager()->Add(
							new CItem2(X(),
								Y(),
								TIPSIZE, TIPSIZE, CApplication::Texture6()));
					}
				}
				else
				{
					if ((rand() % 100) < 25)
					{
						CApplication::CharacterManager()->Add(
							new CItem11(X(),
								Y(),
								TIPSIZE, TIPSIZE, CApplication::Texture6()));
					}
				}
				return;
			}
		}
		break;
	}
}

CEnemy7::CEnemy7(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w, h);
	Texture(pt, TEXCOORDY);
	mTag = ETag::EENEMY;
	//X軸速度の初期値を移動速度にする
	mVx = VELOCITY;
	//敵数に1加算する
	sNum++;
}

void CEnemy7::Update()
{
	if (CItem3::mItem3 > (rand() % 100))
	{
		mEnabled = false;
	}
	if (CItem2::mItem2 > (rand() % 100))
	{
		mEnabled = false;
	}
	if (CItem::mItem > (rand() % 100))
	{
		mEnabled = false;
	}
	if (CItem11::mItem11 > (rand() % 100))
	{
		mEnabled = false;
	}
	switch (mState)
	{
	case EState::ECRY:
		//泣く画像を設定
		Texture(Texture(), TEXCOORDEX);
		break;
	case EState::EMOVE:
		//X軸速度分、X座標を更新する
		//X(X() + mVx);
		const int PITCH = 32;//画像を切り替える間隔
		if ((int) X()% PITCH < PITCH / 2)
		{
			if (mVx < 0.0f) //左へ移動
			{
				//左向き１を設定
				Texture(Texture(), TEXCOORD);
				if (mState == EState::ECRY)
				{
					Texture(Texture(), TEXCOORDEX);
				}
			}
			else
			{
				//通常の画像を設定
				Texture(Texture(), TEXCOORD);
				if (mState == EState::ECRY)
				{
					Texture(Texture(), TEXCOORDEX);
				}
			}
		}
		else
		{
			if (mVx < 0.0f) //左へ移動
			{
				//左向き2を設定
				Texture(Texture(), TEXCOORDEX);
				if (mState == EState::ECRY)
				{
					Texture(Texture(), TEXCOORDEX);
				}
			}
			else
			{
				//2番目の画像を設定
				Texture(Texture(), TEXCOORD);
				if (mState == EState::ECRY)
				{
					Texture(Texture(), TEXCOORDEX);
				}
			}
		}
		break;
	}
}