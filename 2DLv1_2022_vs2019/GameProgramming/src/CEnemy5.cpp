#include"CEnemy5.h"
#include"CAttack.h"
#include "CApplication.h"

#define TEXCOORD 245, 345, 310, 220	//�e�N�X�`���}�b�s���O
#define TEXCOORDEX 345, 245, 310, 220

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
				if (mState != EState::EANNIHILATION)
				{
					sNum--;
				}
				mEnabled = false;
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

CEnemy5::CEnemy5(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w, h);
	Texture(pt, TEXCOORD);
	mTag = ETag::EENEMY;
	//X�����x�̏����l���ړ����x�ɂ���
	mVx = VELOCITY;
	//�G����1���Z����
	sNum++;
}

void CEnemy5::Update()
{
	switch(mState)
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
				Texture(Texture(), TEXCOORDEX);
			}
			else
			{
				//�ʏ�̉摜��ݒ�
				Texture(Texture(), TEXCOORD);
			}
		}
		/*else
		{*/
			//if (mVx < 0.0f) //���ֈړ�
			//{
			//	//������2��ݒ�
			//	Texture(Texture(), TEXCOORD);
			//}
			//else
			//{
			//	//2�Ԗڂ̉摜��ݒ�
			//	Texture(Texture(), TEXCOORDEX);
			//}
		//}
		break;
	}
}