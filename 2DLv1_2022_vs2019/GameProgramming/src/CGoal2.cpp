#include "CGoal2.h"

#define TEXCOORD 195.0f, 280.0f, 418.0f, 330.0f//テクスチャ座標

//int CGoal::sGoal = 0;
//
//void CGoal::Goal(int goal)
//{
//	sGoal = goal;
//}
//
//int CGoal::Goal()
//{
//	return sGoal;
//}

void CGoal2::Collision(CCharacter* m, CCharacter* o)
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
				//mEnabled = false;
				//sGoal--;
			}
		}
		break;
	}
}

//int CGoal::Goal ()
//{
//	return sGoal;
//}

CGoal2::CGoal2(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w, h);
	Texture(pt, TEXCOORD);
	mTag = ETag::EGOAL;
	//sGoal++;
}