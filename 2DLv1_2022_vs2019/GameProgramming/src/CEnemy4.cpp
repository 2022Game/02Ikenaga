#include "CEnemy4.h"
#include"CItem.h"
#include"CItem2.h"
#include"CItem3.h"
#include"CItem11.h"
#include"CAttack.h"
#include "CApplication.h"
#include"stdio.h"
#include "stdlib.h"
#include"time.h"

#define TEXCOORD  120, 70, 370, 320	//�e�N�X�`���}�b�s���O
#define TEXCRY 196, 216, 190, 160	//�e�N�X�`���}�b�s���O
#define TEXCOORD2  40,85,53,17	//�E����2
#define TEXLEFT1  85, 40, 53, 17	//������1
#define TEXLEFT2  40, 1, 53, 17	//������2

int CEnemy4::sNum = 0;

void CEnemy4::Num(int num)
{
	sNum = num;
}

int CEnemy4::Num()
{
	return sNum;
}

void CEnemy4::Collision()
{
	CApplication::CharacterManager()->Collision(this);
}

void CEnemy4::Collision(CCharacter* m, CCharacter* o)
{
	//�߂荞�ݒ����ϐ���錾����
	float x, y;
	switch (o->Tag())
	{
	case ETag::ETURN:
		//�܂�Ԃ��ɓ���������
		if (CRectangle::Collision(o, &x, &y))
		{
			//�߂荞�܂Ȃ��ʒu�܂Ŗ߂�
			X(X() + x);
			Y(Y() + y);
			//X�����x�𔽓]������
			mVx = -mVx;
		}
		break;
	case ETag::EENEMY:
		break;
	case ETag::EPLAYER:
		if (CRectangle::Collision(o))
		{
			if (o->State() == EState::EJUMP)
			{
				//�G��1���Z
				if (mState != EState::EANNIHILATION)
				{
					sNum--;
				}
				mEnabled = false;
				srand((unsigned int)time(nullptr));
				bool d_flag = false;
				int num_pro_d = rand() % 100 + 1;
				int num_pro_m = 100 - num_pro_d;
				//int num_pro_h = 100 - num_pro_d;
				//int num_pro_v = 100 - num_pro_d;
				double tmp_random = (double)rand() / RAND_MAX;
				if (tmp_random < (double)num_pro_d / 100)
				{
					d_flag = true;
				}
				else if ((rand() % 100) < 50) {
					{
						CApplication::CharacterManager()->Add(
							new CItem3(X(),
								Y(),
								TIPSIZE, TIPSIZE, CApplication::Texture6()));
					}
				}
				else if ((rand() % 100) < 25) {
					{
						CApplication::CharacterManager()->Add(
							new CItem2(X(),
								Y(),
								TIPSIZE, TIPSIZE, CApplication::Texture6()));
					}
				}
				else
				{
					if ((rand() % 100) < 25)
					{
						CApplication::CharacterManager()->Add(
							new CItem11(X(),
								Y(),
								TIPSIZE, TIPSIZE, CApplication::Texture6()));
					}
				}
				return;
			}
		}
		break;
	case ETag::EATTACK:
		if (CRectangle::Collision(o))
		{
			if (o->State() != EState::EATTACK)
			{
				//�G��1���Z
				if (mState != EState::EANNIHILATION)
				{
					//sNum--;
				}
				mEnabled = false;
				mEnabled = false;
				srand((unsigned int)time(nullptr));
				bool d_flag = false;
				int num_pro_d = rand() % 100 + 1;
				int num_pro_m = 100 - num_pro_d;
				//int num_pro_h = 100 - num_pro_d;
				//int num_pro_v = 100 - num_pro_d;
				double tmp_random = (double)rand() / RAND_MAX;
				if (tmp_random < (double)num_pro_d / 100)
				{
					d_flag = true;
				}
				else if ((rand() % 100) < 50) {
					{
						CApplication::CharacterManager()->Add(
							new CItem3(X(),
								Y(),
								TIPSIZE, TIPSIZE, CApplication::Texture6()));
					}
				}
				else if ((rand() % 100) < 25) {
					{
						CApplication::CharacterManager()->Add(
							new CItem2(X(),
								Y(),
								TIPSIZE, TIPSIZE, CApplication::Texture6()));
					}
				}
				else
				{
					if ((rand() % 100) < 25)
					{
						CApplication::CharacterManager()->Add(
							new CItem11(X(),
								Y(),
								TIPSIZE, TIPSIZE, CApplication::Texture6()));
					}
				}
				return;
			}
		}
		break;
	}
}

CEnemy4::CEnemy4(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w, h);
	Texture(pt, TEXCOORD);
	mTag = ETag::EENEMY;
	//X�����x�̏����l���ړ����x�ɂ���
	mVx = VELOCITY;
	//�G����1���Z����
	sNum++;
}

void CEnemy4::Update()
{
	if (CItem3::mItem3 > (rand() % 100))
	{
		mEnabled = false;
	}
	if (CItem2::mItem2 > (rand() % 100))
	{
		mEnabled = false;
	}
	if (CItem::mItem > (rand() % 100))
	{
		mEnabled = false;
	}
	if (CItem11::mItem11 > (rand() % 100))
	{
		mEnabled = false;
	}
}