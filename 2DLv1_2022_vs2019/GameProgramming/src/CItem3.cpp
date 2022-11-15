#include "CItem3.h"
#include "CApplication.h"

#define TEXCOORD1 325,280,150,190   //ポイント(コイン)

void CItem3::Collision()
{
	CApplication::CharacterManager()->Collision(this);
}

void CItem3::Collision(CCharacter* m, CCharacter* o)
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

void CItem3::Update()
{
	//通常の画像を設定
	Texture(Texture(), TEXCOORD1);
}

CItem3::CItem3(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w, h);
	Texture(pt, TEXCOORD1);
	mTag = ETag::EITEM3;
}