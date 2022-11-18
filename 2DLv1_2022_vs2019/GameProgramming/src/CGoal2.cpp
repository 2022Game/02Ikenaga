#include "CGoal2.h"

#define TEXCOORD 195.0f, 280.0f, 418.0f, 330.0f//�e�N�X�`�����W

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
	//�߂荞�ݒ����ϐ���錾����
	float x, y;
	switch (o->Tag())
	{
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