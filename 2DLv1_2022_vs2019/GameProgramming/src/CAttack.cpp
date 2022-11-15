#include "CAttack.h"
#include"CPlayer2.h"
#include "CApplication.h"
#define KODOMO "117545.png"
#define KODOMO1 370, 490, 460, 370//�e�N�X�`�����W

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
//	//Y�����x�̏����l���ړ����x�ɂ���
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
			//�G�̏Փ˔�������s
			o->Collision(o, m);
			X(X() + x);
			Y(Y() + y);
			if (x != 0.0f)
			{
				//X�����x��0�ɂ���
				mVx = 0.0f;
				if (x > 0.0f)
				{
					mState = EState::EMOVE;
				}
				else
				{	//�W�����v�łȂ���΋���
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

