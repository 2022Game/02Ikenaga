#include "CItem11.h"
#include "CApplication.h"
#include <iostream>
#include <thread>
#include <chrono>
#define TEXCOORD 36,83,263,221     //���v
#define TEXCOORD2 36,83,263,221    

void CItem11::Collision()
{
	CApplication::CharacterManager()->Collision(this);
}

void CItem11::Collision(CCharacter* m, CCharacter* o)
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
				//mTime--;
			}
		}
		break;
	}
}
void CItem11::Update()
{
	//�ʏ�̉摜��ݒ�
	Texture(Texture(), TEXCOORD);
}

CItem11::CItem11(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w, h);
	Texture(pt, TEXCOORD);
	mTag = ETag::EITEM;
	/*int mTime = CGame::mTime;
	if (mTime > 500) 
	{
		mEnabled = false;
	}*/
}