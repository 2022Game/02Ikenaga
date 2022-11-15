#include "CItem.h"
#include "CApplication.h"

#define TEXCOORD 40,90,263,220      //���v
//#define POINT 0
//#define AITEM1                     

//int CItem::sPoint = 0;	//POINT
//
//int  CItem::Point()
//{
//	return sPoint;
//}

void CItem::Collision()
{
	CApplication::CharacterManager()->Collision(this);
}

void CItem::Collision(CCharacter* m, CCharacter* o)
{
	//�߂荞�ݒ����ϐ���錾����
	float x, y;
	switch (o->Tag())
	{
	//case ETag::EITEM:
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
		}
	case ETag::EPLAYER:
		if (CRectangle::Collision(o))
		{
			if (o->Tag() == ETag::EPLAYER)
			{
				mEnabled = false;
				mTime--;
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
void CItem::Update()
{
	//�ʏ�̉摜��ݒ�
	Texture(Texture(), TEXCOORD);
	//Texture(Texture(), AITEM);
	//mTime--;
}

CItem::CItem(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w, h);
	Texture(pt, TEXCOORD);
	mTag = ETag::EITEM;
	//sPoint = POINT;
}