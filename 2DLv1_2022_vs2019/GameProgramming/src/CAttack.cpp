#include "CAttack.h"
#include"CPlayer2.h"
#include "CApplication.h"
#define KODOMO "117545.png"
#define KODOMO 370, 490, 460, 370//テクスチャ座標

void CAttack::Collision()
{
	CApplication::CharacterManager()->Collision(this);
}

CAttack::CAttack()
{
	mState = EState::ESTOP;
	mTag = ETag::EATTACK;
}

CAttack::CAttack(float x, float y, float w, float h, float l, float r, float b, float t, CTexture* pt)
{
	Set(x, y, w, h);
	Texture(pt, l, r, b, t);
	Texture(pt, KODOMO);
	mState = EState::EMOVE;
	mTag  = ETag::EATTACK;
}

CAttack::CAttack(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w, h);
	Texture(pt, KODOMO);
	//mTag = ETag::EPLAYER;
	//Y軸速度の初期値を移動速度にする
	; mVx = SOKUDO;
}

void CAttack::Update()
{
		if (mState == EState::EMOVE)
		{
			float x = X() + W();
			if (x > 800.0f)
			{
				x = 5.0f;
			}
			/*if (y > 600)
			{
				y = 700.0f;
			}*/
			X(x);
		}
}
bool CAttack::Collision(CRectangle* rect)
{
	if (CRectangle::Collision(rect))
	{
		mState = EState::ESTOP;
		return true;
	}
	return false;
}

void CAttack::Collision(CCharacter* m, CCharacter* o)
{
	switch (o->Tag())
	{
	case ETag::EATTACK:
	case ETag::EPLAYER:
		break;
	default:
		if (CRectangle::Collision(o))
		{
			mState = EState::ESTOP;
			mEnabled = false;
		}
	}
}

