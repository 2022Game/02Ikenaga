#include "CItem2.h"
#include "CApplication.h"
#include"stdio.h"
#include "stdlib.h"
#include"time.h"

//#define RAND_MAX 0x7fff

#define TEXCOORD1  151,190,262,225       //�C�A��(�X�s�[�h�A�b�v)//�|�C���g(�R�C��)

void CItem2::Collision()
{
	CApplication::CharacterManager()->Collision(this);
}

void CItem2::Collision(CCharacter* m, CCharacter* o)
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
//			//�����l�̕\��
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
	srand((unsigned int)time(NULL));
}