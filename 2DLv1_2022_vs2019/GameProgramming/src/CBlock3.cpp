#include "CBlock3.h"
#include "CApplication.h"

#define TEXCOORD 315.0f, 290.0f, 289.0f, 319.0f//310.0f, 390.0f, 150.0f, 190.0f//�e�N�X�`�����W

//int CBlock3::sNum = 0;

void CBlock3::Collision()
{
	CApplication::CharacterManager()->Collision(this);
}


void CBlock3::Collision(CCharacter* m, CCharacter* o)
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
	case ETag::EBLOCK3:
		break;
	case ETag::EPLAYER:
		if (CRectangle::Collision(o))
		{
			if (o->State() == EState::EJUMP)
			{
				mState = EState::EMOVE;
				//mEnabled = false;
			}
			/*else
			{
				mTag = ETag::EBLOCK3;
			}*/
		}
		break;
	//case ETag::EBLOCK:
	//	break;
	/*case ETag::EPLAYER:
		break;*/
	}
}

CBlock3::CBlock3(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w, h);
	Texture(pt, TEXCOORD);
	mTag = ETag::EBLOCK3;
	//Y�����x�̏����l���ړ����x�ɂ���
	mVy = SOKUDO;
}

void CBlock3::Update()
{
	switch (mState)
	{
	case EState::ECRY:
		//�����摜��ݒ�
		//Texture(Texture(), TEXCRY);
		break;
	case EState::EMOVE:
		//Y�����x���AY���W���X�V����
		Y(Y() + mVy);
		const int PITCH = 32;//�摜��؂�ւ���Ԋu
		if ((int)Y() % PITCH < PITCH / 2)
		{
			if (mVy < 0.0f) //���ֈړ�
			{
				//�������P��ݒ�
				//Texture(Texture(), TEXLEFT1);
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
				//Texture(Texture(), TEXLEFT2);
			}
			else
			{
				//2�Ԗڂ̉摜��ݒ�
				//Texture(Texture(), TEXCOORD2);
			}
		}
		break;
	}
}