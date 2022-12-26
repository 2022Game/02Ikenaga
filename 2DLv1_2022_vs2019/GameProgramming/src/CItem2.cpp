#include "CItem2.h"
#include "CApplication.h"
#include"stdio.h"
#include "stdlib.h"
#include"time.h"

//#define RAND_MAX 0x7fff

#define TEXCOORD1  151,190,262,225       //靴、足(スピードアップ)

void CItem2::Collision()
{
	CApplication::CharacterManager()->Collision(this);
}

void CItem2::Collision(CCharacter* m, CCharacter* o)
{
	//めり込み調整変数を宣言する
	float x, y;
	switch (o->Tag())
	{
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
					//mState = EState::ESTOP;
				}
			}
			break;
			//default:
			//	if (CRectangle::Collision(o))
			//	{
			//		//mState = EState::ESTOP;
			//		//mEnabled = false;
			//	}
	}
}
void CItem2::Update()
{

	Texture(Texture(), TEXCOORD1);
}

//void CItem2::Render()
//{
//	//Texture(Texture(), TEXCOORD1);
//	//char str[2];
//	int main(void);
//	{
//		int i;
//
//		//srand(10);
//		srand((unsigned int)time(nullptr));
//
//		for (i = 0; i < 10; i++)
//		{
//			//乱数値の表示
//			printf("%d\n", 0 + rand() % 10);
//			//printf("%lf\n", rand() / (double)RAND_MAX);
//			//return ;
//		}
//
//		return ;
//	}
//}

CItem2::CItem2(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w, h);
	 Texture(pt, TEXCOORD1);
	mTag = ETag::EITEM2;
	//srand((unsigned int)time(NULL));
}