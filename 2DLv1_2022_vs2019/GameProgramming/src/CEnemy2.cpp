#include "CEnemy2.h"
#include"CAttack.h"
#include "CApplication.h"
#include"stdio.h"
#include "stdlib.h"
#include"time.h"


#define TEXCOORD  145, 192, 143, 101	//�e�N�X�`���}�b�s���O
#define TEXCRY 196, 216, 190, 160	//�e�N�X�`���}�b�s���O
#define TEXCOORD2 240,289,143,101	//�E����2
#define TEXLEFT1 192, 145, 143, 101	//������1
#define TEXLEFT2  289,240,143,101	//������2

//int main(void)
//{ 
//	char ch;
//	char item;
//	int i;
//	srand((unsigned int)time(nullptr));
//	while (ch != 'Q') {
//		i = (rand() % 5) + 1;
//		printf(item[i]);
//		ch = getchar();
//	}
//}

int CEnemy2::sNum = 0;

void CEnemy2::Num(int num)
{
	sNum = num;
}

int CEnemy2::Num()
{
	return sNum;
}

void CEnemy2::Collision()
{
	CApplication::CharacterManager()->Collision(this);
}

void CEnemy2::Collision(CCharacter* m, CCharacter* o)
{
	//�߂荞�ݒ����ϐ���錾����
	float x, y;
	switch (o->Tag())
	{
	case ETag::EBLOCK:
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
				if (mState != EState::ECRY)
				{
					sNum--;
				}
				mEnabled = false;
				//�A�C�e���̃|�C���g�𐶐����āA�L�����N�^�}�l�[�W���ɒǉ�
				CApplication::CharacterManager()->Add(
					new CItem3(X(),
						Y(),
						TIPSIZE, TIPSIZE, CApplication::Texture6()));
			}
		}
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
					//�A�C�e���̃|�C���g�𐶐����āA�L�����N�^�}�l�[�W���ɒǉ�
					CApplication::CharacterManager()->Add(
						new CItem3(X(),
							Y(),
							TIPSIZE, TIPSIZE, CApplication::Texture6()));
				}
			}
			break;
	case ETag::EATTACK:
		if (CRectangle::Collision(o))
		{
			if (o->State() != EState::EATTACK)
			{
				//�G��1���Z
				if (mState != EState::ECRY)
				{
					//sNum--;
				}
				mEnabled = false;
				//char ch;
				//char item;
				//int  i;
				//while (ch != 'Q') {
				//	i = (rand() % 5) + 1;
				//	//item��\��
				//	printf(item[i]);
				//	//ch��Q����������I��郋�[�v
				//	ch = getchar();
				//}
				//i= (rand() % 100) + 1;
				//	if (i <= 20) {
				//		printf(item[0]);
				//	}
				//	if (i >= 20) printf(item[0]);
				//	if (i >= 20 || i <= 40) {
				//		printf(item[1]);
				//	}
				//	else(i <= 40 || i >= 50) {
				//		printf(item[2]);
				//	}
				//	else(i <= 60 || i >= 65) {
				//		printf(item[3]);
				//	}
				//	else {
						//printf(item[4]);
				     if   (mItem3  = (rand() % 100) + 1)
						{
						 mItem3 = 0 + rand() % 10;
							{
								//�A�C�e���̃|�C���g�𐶐����āA�L�����N�^�}�l�[�W���ɒǉ�
								CApplication::CharacterManager()->Add(
									new CItem3(X(),
										Y(),
										TIPSIZE, TIPSIZE, CApplication::Texture6()));
							}
						}
						if(	mItem2 = (rand()% 100)+1)
							{
								mItem2 = 0 + rand() % 30;
								{
									//�A�C�e���̃|�C���g�𐶐����āA�L�����N�^�}�l�[�W���ɒǉ�
									CApplication::CharacterManager()->Add(
										new CItem2(X(),
											Y(),
											TIPSIZE, TIPSIZE, CApplication::Texture6()));
								}
							}
					//}
			}
		}
		break;
	//default:
	//	if (CRectangle::Collision(o))
	//	{
	//		//mState = EState::ESTOP;
 //			mEnabled = false;
	//	}
	}
}

int CEnemy2::mItem3 = 0;
int CEnemy2::mItem2 = 0;

CEnemy2::CEnemy2(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w, h);
	Texture(pt, TEXCOORD);
	mTag = ETag::EENEMY;
	//X�����x�̏����l���ړ����x�ɂ���
	mVx = VELOCITY;
	//�G����1���Z����
	sNum++;
	srand((unsigned int)time(NULL));
}

void CEnemy2::Update()
{
	if (CEnemy2::mItem3 >10)
	{
		mEnabled = false;
	}
	//if (CEnemy2::mItem3 > 10)
	//{
	//	//�A�C�e���̃|�C���g�𐶐����āA�L�����N�^�}�l�[�W���ɒǉ�
	//	CApplication::CharacterManager()->Add(
	//		new CItem3(X(),
	//			Y(),
	//			TIPSIZE, TIPSIZE, CApplication::Texture6()));
	//}
	if (CEnemy2::mItem2 > 30)
	{
		//�A�C�e���̃|�C���g�𐶐����āA�L�����N�^�}�l�[�W���ɒǉ�
		CApplication::CharacterManager()->Add(
			new CItem2(X(),
				Y(),
				TIPSIZE, TIPSIZE, CApplication::Texture6()));
	}
	if (CEnemy2::mItem2 > 30)
	{
		mEnabled = false;
	}

	switch (mState)
	{
		case EState::ECRY:
		//�����摜��ݒ�
		//Texture(Texture(), TEXCRY);
		break;
	case EState::EMOVE:
		//X�����x���AX���W���X�V����
		X(X() + mVx);
		const int PITCH = 32;//�摜��؂�ւ���Ԋu
		if ((int)X() % PITCH < PITCH / 2)
		{
			if (mVx < 0.0f) //���ֈړ�
			{
				//�������P��ݒ�
				Texture(Texture(), TEXLEFT1);
			}
			else
			{
				//�ʏ�̉摜��ݒ�
				Texture(Texture(), TEXCOORD);
			}
		}
		else
		{
			if (mVx < 0.0f) //���ֈړ�
			{
				//������2��ݒ�
				Texture(Texture(), TEXLEFT2);
			}
			else
			{
				//2�Ԗڂ̉摜��ݒ�
				Texture(Texture(), TEXCOORD2);
			}
		}
		break;
	}
}
