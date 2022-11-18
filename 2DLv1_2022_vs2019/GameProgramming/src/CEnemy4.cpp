#include "CEnemy4.h"
#include"CItem.h"
#include"CItem2.h"
#include"CItem3.h"
#include"CAttack.h"
#include "CApplication.h"

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
				//�A�C�e���̎��v�𐶐����āA�L�����N�^�}�l�[�W���ɒǉ�
				CApplication::CharacterManager()->Add(
					new CItem(X(),
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
				if (mState != EState::EANNIHILATION)
				{
					//sNum--;
				}
				mEnabled = false;
				CApplication::CharacterManager()->Add(
					new CItem(X(),
						Y(),
						TIPSIZE, TIPSIZE, CApplication::Texture6()));
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
}