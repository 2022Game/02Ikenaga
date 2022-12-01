#include "CAttack.h"
#include"CPlayer2.h"
#include "CApplication.h"
#define KODOMO "1175450110.png"
#define KODOMO1 370, 490, 460, 370//テクスチャ座標

void CAttack::Collision()
{
	CApplication::CharacterManager()->Collision(this);
}

//CAttack::CAttack()
//{
//	mState = EState::ESTOP;
//	mTag = ETag::EATTACK;
//}

CAttack::CAttack(float x, float y, float w, float h, float l, float r, float b, float t, CTexture* pt)
{
	Set(x, y, w, h);
	Texture(pt, l, r, b, t);
	Texture(pt, KODOMO1);
	//mState = EState::EMOVE;
	mTag  = ETag::EATTACK;
	//mState = EState::EATTACK;
}

//CAttack::CAttack(float x, float y, float w, float h, CTexture* pt)
//{
//	Set(x, y, w, h);
//	Texture(pt, KODOMO1);
//	mTag = ETag::EATTACK;
//	//Y軸速度の初期値を移動速度にする
//	; mVx = SOKUDO;
//}

void CAttack::Update()
{
		if (mState == EState::EMOVE)
		{
			float x = X() + W();
			if (x > 800.0f)
			{
				x = 0.0f;
			}
			/*if (y > 600)
			{
				y = 700.0f;
			}*/
			X(x);
		}
}
//bool CAttack::Collision(CRectangle* rect)
//{
//	if (CRectangle::Collision(rect))
//	{
//		mState = EState::ESTOP;
//		return true;
//	}
//	return false;
//}

void CAttack::Collision(CCharacter* m, CCharacter* o)
{
	float x, y;
	switch (o->Tag())
	{
	case ETag::EENEMY:
		if (CRectangle::Collision(o, &x, &y))
		{
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
				mEnabled = false;
			}
			break;
		}
	case ETag::EATTACK:
		break;
	case ETag::EPLAYER:
		break;
	case ETag::ETURN:
		break;
	default:
		if (CRectangle::Collision(o))
		{
			//mState = EState::ESTOP;
			mEnabled = false;
		}
	}
}

