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
		//default:
		//	if (CRectangle::Collision(o))
		//	{
		//		//mState = EState::ESTOP;
		//		mEnabled = false;
		//	}
	}
}
void CItem11::Update()
{
	//�ʏ�̉摜��ݒ�
	Texture(Texture(), TEXCOORD);
	//Texture(Texture(), AITEM);
	//mTime--;
	//int ime_t ;
	//int last;
	//ime_t; last = time(0);
	//time_t next;
	//int pastSec = 0;
	//int a = 1;
	//while (a)
	//{
	//	if (time(&next) != last)
	//	{
	//		last = next;
	//		pastSec++;
	//		printf("%d �b�o��\n", pastSec);
	//		if (pastSec == 10) break; /*** �ǉ� */
	//	}
	//}
	//return ;
	/*if (CGame::mTime > 700)
	{
		CApplication::CharacterManager()->Add(
			new CItem(X(),
				Y(),
				TIPSIZE, TIPSIZE, CApplication::Texture6()));
	}*/
	//int mTime = CGame::mTime;
	//int cout =500;
	////int endl;
	////cout <<500 << endl;
	//while (CGame::mTime > cout)
	//{
	//	 mEnabled = false;
	//	 return;
	//}
	//int mTime = CGame::mTime;
	//int mItem11 = false;
	//while (mTime > 100) {
	//	if (mItem11 == false) {
	//		//1�񂾂��s������
	//		mItem11 = true;
	//		mEnabled = false;
	//	}
		//return ;
		//����s������
		/*CApplication::CharacterManager()->Add(
			new CItem11(X(),
				Y(),
				TIPSIZE, TIPSIZE, CApplication::Texture6()));*/
				//}
				/*int mTime = CGame::mTime;
				if (mTime > 300)
				{
					mEnabled = false;
					return;
				}*/
				//int mTime = CGame::mTime;
				////int  mEnabled = false;
				//while (mTime == CGame::mTime) {
				//	//if (mEnabled == false) {
				//		//1�񂾂��s������
				//		if (mTime > 100)
				//		{
				//			mEnabled = false;
				//		}
					//}
					//����s������
				//}
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