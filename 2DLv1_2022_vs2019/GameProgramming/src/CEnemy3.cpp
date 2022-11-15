#include "CEnemy3.h"
#include"CItem.h"
#include"CItem2.h"
#include"CItem3.h"
#include"CAttack.h"
#include "CApplication.h"

#define TEXCOORD  1, 40, 53, 17	//テクスチャマッピング
#define TEXCRY 196, 216, 190, 160	//テクスチャマッピング
#define TEXCOORD2  40,85,53,17	//右向き2
#define TEXLEFT1  85, 40, 53, 17	//左向き1
#define TEXLEFT2  40, 1, 53, 17	//左向き2

int CEnemy3::sNum = 0;

void CEnemy3::Num(int num)
{
	sNum = num;
}

int CEnemy3::Num()
{
	return sNum;
}

void CEnemy3::Collision()
{
	CApplication::CharacterManager()->Collision(this);
}

void CEnemy3::Collision(CCharacter* m, CCharacter* o)
{
	//めり込み調整変数を宣言する
	float x, y;
	switch (o->Tag())
	{
	case ETag::ETURN:
		//折り返しに当たった時
		if (CRectangle::Collision(o, &x, &y))
		{
			//めり込まない位置まで戻す
			X(X() + x);
			Y(Y() + y);
			//X軸速度を反転させる
			mVx = -mVx;
		}
		break;
	case ETag::EENEMY:
		break;
	case ETag::EPLAYER:
		if (CRectangle::Collision(o))
		{
			if (o->State() == EState::EJUMP)
			{
				//敵数1減算
				if (mState != EState::EANNIHILATION)
				{
					sNum--;
				}
				mEnabled = false;
				//アイテムの時計を生成して、キャラクタマネージャに追加
				CApplication::CharacterManager()->Add(
					new CItem(X(),
						Y(),
						TIPSIZE, TIPSIZE, CApplication::Texture6()));
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
					CApplication::CharacterManager()->Add(
						new CItem(X(),
							Y(),
							TIPSIZE, TIPSIZE, CApplication::Texture6()));
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

CEnemy3::CEnemy3(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w, h);
	Texture(pt, TEXCOORD);
	mTag = ETag::EENEMY;
	//X軸速度の初期値を移動速度にする
	mVx = VELOCITY;
	//敵数に1加算する
	sNum++;
}

void CEnemy3::Update()
{
	switch (mState)
	{
	case EState::EANNIHILATION:
		//泣く画像を設定
		//Texture(Texture(), TEXCRY);
		break;
	case EState::EMOVE:
		//X軸速度分、X座標を更新する
		X(X() + mVx);
		const int PITCH = 32;//画像を切り替える間隔
		if ((int)X() % PITCH < PITCH / 2)
		{
			if (mVx < 0.0f) //左へ移動
			{
				//左向き１を設定
				Texture(Texture(), TEXLEFT1);
			}
			else
			{
				//通常の画像を設定
				Texture(Texture(), TEXCOORD);
			}
		}
		else
		{
			if (mVx < 0.0f) //左へ移動
			{
				//左向き2を設定
				Texture(Texture(), TEXLEFT2);
			}
			else
			{
				//2番目の画像を設定
				Texture(Texture(), TEXCOORD2);
			}
		}
		break;
	}
}