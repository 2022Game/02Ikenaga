#include"CEnemy5.h"
#include"CItem.h"
#include"CItem2.h"
#include"CItem3.h"
#include"CItem11.h"
#include"CAttack.h"
#include "CApplication.h"
#include"stdio.h"
#include "stdlib.h"
#include"time.h"

#define TEXCOORD 147, 164, 97, 72	//�e�N�X�`���}�b�s���O
#define TEXCOORDEX 172, 188, 97, 72
#define TEXCOORDY 195,213,97,72

int CEnemy5::sNum = 0;

void CEnemy5::Num(int num)
{
	sNum = num;
}

int CEnemy5::Num()
{
	return sNum;
}

void CEnemy5::Collision()
{
	CApplication::CharacterManager()->Collision(this);
}

void CEnemy5::Collision(CCharacter* m, CCharacter* o)
{
	//�߂荞�ݒ����ϐ���錾����
	float x, y;
	switch (o->Tag())
	{
	case ETag::EBLOCK4:
		//�܂�Ԃ��ɓ���������
		if (CRectangle::Collision(o, &x, &y))
		{
			//�߂荞�܂Ȃ��ʒu�܂Ŗ߂�
			X(X() + x);
			Y(Y() + y);
			//X�����x�𔽓]������
			mVy = -mVy;
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
		if (CRectangle::Collision(o))
		{
			if (o->State() == EState::EUP)
			{
				//�G��1���Z
				if (mState != EState::ECRY)
				{
					sNum--;
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

CEnemy5::CEnemy5(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w, h);
	Texture(pt, TEXCOORDY);
	mTag = ETag::EENEMY;
	//X�����x�̏����l���ړ����x�ɂ���
	mVy = VELOCITY;
	//�G����1���Z����
	sNum++;
}

void CEnemy5::Update()
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
	switch(mState)
	{
	case EState::ECRY:
		//�����摜��ݒ�
		//Texture(Texture(), TEXCRY);
		break;
	case EState::EMOVE:
		//X�����x���AX���W���X�V����
		Y(Y() + mVy);
		const int PITCH = 32;//�摜��؂�ւ���Ԋu
		if ((int)Y() % PITCH < PITCH / 2)
		{
			if (mVy < 0.0f) //���ֈړ�
			{
				//�������P��ݒ�
				Texture(Texture(), TEXCOORDEX);
				if (mState == EState::EMOVE)
				{
					Texture(Texture(), TEXCOORDY);
				}
			}
			else
			{
				//�ʏ�̉摜��ݒ�
				Texture(Texture(), TEXCOORD);
			}
		}
		else
		{
			if (mVy < 0.0f) //���ֈړ�
			{
				//������2��ݒ�
				Texture(Texture(), TEXCOORD);
			}
			else
			{
				//2�Ԗڂ̉摜��ݒ�
				Texture(Texture(),TEXCOORDEX);
				if (mState == EState::EMOVE)
				{
					Texture(Texture(), TEXCOORDY);
				}
			}
		}
		break;
	}
}