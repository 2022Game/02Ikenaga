#include "CEnemy2.h"
#include"CAttack.h"
#include "CApplication.h"
#include"stdio.h"
#include "stdlib.h"
#include"time.h"


#define TEXCOORD  145, 192, 143, 101	//テクスチャマッピング
#define TEXCRY 196, 216, 190, 160	//テクスチャマッピング
#define TEXCOORD2 240,289,143,101	//右向き2
#define TEXLEFT1 192, 145, 143, 101	//左向き1
#define TEXLEFT2  289,240,143,101	//左向き2

//int main(void)
//{ 
//	char ch;
//	char item;
//	int i;
//	srand((unsigned int)time(nullptr));
//	while (ch != 'Q') {
//		i = (rand() % 5) + 1;
//		printf(item[i]);
//		ch = getchar();
//	}
//}

int CEnemy2::sNum = 0;

void CEnemy2::Num(int num)
{
	sNum = num;
}

int CEnemy2::Num()
{
	return sNum;
}

void CEnemy2::Collision()
{
	CApplication::CharacterManager()->Collision(this);
}

void CEnemy2::Collision(CCharacter* m, CCharacter* o)
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
			mVx = -mVx;
			//-mVx <=mVx;
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
				if (mState != EState::ECRY)
				{
					sNum--;
				}
				mEnabled = false;
				//アイテムのポイントを生成して、キャラクタマネージャに追加
				CApplication::CharacterManager()->Add(
					new CItem3(X(),
						Y(),
						TIPSIZE, TIPSIZE, CApplication::Texture6()));
			}
		}
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
					//アイテムのポイントを生成して、キャラクタマネージャに追加
					CApplication::CharacterManager()->Add(
						new CItem3(X(),
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
				if (mState != EState::ECRY)
				{
					//sNum--;
				}
				mEnabled = false;
				//char ch;
				//char item;
				//int  i;
				//while (ch != 'Q') {
				//	i = (rand() % 5) + 1;
				//	//itemを表示
				//	printf(item[i]);
				//	//chにQが入ったら終わるループ
				//	ch = getchar();
				//}
				//i= (rand() % 100) + 1;
				//	if (i <= 20) {
				//		printf(item[0]);
				//	}
				//	if (i >= 20) printf(item[0]);
				//	if (i >= 20 || i <= 40) {
				//		printf(item[1]);
				//	}
				//	else(i <= 40 || i >= 50) {
				//		printf(item[2]);
				//	}
				//	else(i <= 60 || i >= 65) {
				//		printf(item[3]);
				//	}
				//	else {
						//printf(item[4]);
				     if   (mItem3  = (rand() % 100) + 1)
						{
						 mItem3 = 0 + rand() % 10;
							{
								//アイテムのポイントを生成して、キャラクタマネージャに追加
								CApplication::CharacterManager()->Add(
									new CItem3(X(),
										Y(),
										TIPSIZE, TIPSIZE, CApplication::Texture6()));
							}
						}
						if(	mItem2 = (rand()% 100)+1)
							{
							mItem2 = 0 + rand() % 30;
								{
									//アイテムのポイントを生成して、キャラクタマネージャに追加
									CApplication::CharacterManager()->Add(
										new CItem2(X(),
											Y(),
											TIPSIZE, TIPSIZE, CApplication::Texture6()));
								}
							}
					//}
			}
		}
		break;
	//default:
	//	if (CRectangle::Collision(o))
	//	{
	//		//mState = EState::ESTOP;
 //			mEnabled = false;
	//	}
	}
}

int CEnemy2::mItem3 = 0;
int CEnemy2::mItem2 = 0;

CEnemy2::CEnemy2(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w, h);
	Texture(pt, TEXCOORD);
	mTag = ETag::EENEMY;
	//X軸速度の初期値を移動速度にする
	mVx = VELOCITY;
	//敵数に1加算する
	sNum++;
	srand((unsigned int)time(NULL));
}

void CEnemy2::Update()
{
	if (CEnemy2::mItem3 >10)
	{
		mEnabled = false;
	}
	//if (CEnemy2::mItem3 > 10)
	//{
	//	//アイテムのポイントを生成して、キャラクタマネージャに追加
	//	CApplication::CharacterManager()->Add(
	//		new CItem3(X(),
	//			Y(),
	//			TIPSIZE, TIPSIZE, CApplication::Texture6()));
	//}
	if (CEnemy2::mItem2 > 30)
	{
		//アイテムのポイントを生成して、キャラクタマネージャに追加
		CApplication::CharacterManager()->Add(
			new CItem2(X(),
				Y(),
				TIPSIZE, TIPSIZE, CApplication::Texture6()));
	}
	if (CEnemy2::mItem2 > 30)
	{
		mEnabled = false;
	}

	switch (mState)
	{
		case EState::ECRY:
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
