#include "CPlayer2.h"
#include "CApplication.h"

#define TEXCOORD 30, 66, 100, 50	//�e�N�X�`���}�b�s���O
#define TEXCRY 196, 216, 158, 128	//�e�N�X�`���}�b�s���O
#define GRAVITY (TIPSIZE / 16.0f)	//�d�͉����x16
#define JUMPV0 (TIPSIZE / 1.5f)//�W�����v�̏���
#define JUMPVO2 (TIPSIZE / 1.4f)
#define TEXCOORD2 150,189,340,293 //�E����2
#define TEXCOORD3 510,550,340,293
#define TEXCOORD4 620,1105,340,293
#define TEXLEFT1 190,150,340,293	//������1
#define TEXLEFT2 550,510,340,293	//������2
#define VELOCITY 2.0f	//�ړ����x
#define SOKUDO 1.0f
#define HP 3 //HP�̏����l��3
#define SE_JUMP "res\\jump.wav" //�W�����v�̉����t�@�C��

int CPlayer2::sHp = 0;	//HP

int CPlayer2::Hp()
{
	return sHp;
}

void CPlayer2::Collision()
{
	CApplication::CharacterManager()->Collision(this);
}

void CPlayer2::Collision(CCharacter* m, CCharacter* o)
{
	float x, y;
	switch (o->Tag())
	{
	case ETag::EENEMY:
		if (CRectangle::Collision(o, &x, &y))
		{
			//�G�̏Փ˔�������s
			o->Collision(o, m);
			X(X() + x);
			Y(Y() + y);
			//���n������
			if (y != 0.0f)
			{
				//Y�����x��0�ɂ���
				mVy = 0.0f;
				if (y > 0.0f)
				{
					mState = EState::EMOVE;
				}
				else
				{	//�W�����v�łȂ���΋���
					mState = EState::ECRY;
					/*if (mInvincible == 0)
					{
						mInvincible = 60;
						sHp--;
					}*/
				}
			}
			else
			{	//�W�����v�łȂ���΋���
				mState = EState::ECRY;
				/*if (mInvincible == 0)
				{
					mInvincible = 60;
					sHp--;
				}*/
			}
		}
		break;
	case ETag::EPLAYER:
		break;
	case ETag::EBLOCK:
		if (CRectangle::Collision(o, &x, &y))
		{
			X(X() + x);
			Y(Y() + y);
			//���n������
			if (y != 0.0f)
			{
				//Y�����x��0�ɂ���
				mVy = 0.0f;
				if (y > 0.0f)
				{
					mState = EState::EMOVE;
				}
			}
		}
		break;
	}
}

CPlayer2::CPlayer2(float x, float y, float w, float h, CTexture* pt)
	: mInvincible(0),mVy(0.0f)
{
	Set(x, y, w, h);
	Texture(pt, TEXCOORD);
	mTag = ETag::EPLAYER;
	sHp = HP;
	//�W�����v�����[�h
	mSoundJump.Load(SE_JUMP);
}

void CPlayer2::Update()
{
	//���G���Ԓ��̓J�E���g����������
	if (mInvincible > 0)
	{
		mInvincible--;
	}
	if (mState != EState::EJUMP)
	{
		if (mInput.Key('J'))
		{
			//�W�����v��
			mSoundJump.Play(0.1f);
			mVy = JUMPV0;
			mState = EState::EJUMP;
		}
	}
	if (mInput.Key('A'))
	{
		mVx = -VELOCITY;
		//		float x = X() - 4.0f;
		X(X() + mVx);
	}
	if (mInput.Key('D'))
	{
		mVx = VELOCITY;
		//		float x = X() - 4.0f;
		X(X() + mVx);
	}
	//Y���W��Y�����x��������
	Y(Y() + mVy);
	//Y�����x�ɏd�͂����Z����
	mVy -= GRAVITY;
	if (mState == EState::ECRY)
	{
		//�����摜��ݒ�
		Texture(Texture(), TEXCRY);
		if (mInvincible == 0)
		{
			mInvincible = 60;
			sHp--;
		}
	}
	if (mInvincible > 0)
	{
		mState = EState::ECRY;
	}
	if (mState == EState::EJUMP)
	{
		Texture(Texture(), TEXCOORD4);
	}
	else
	{
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
				Texture(Texture(), TEXCOORD3);
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
	}
}

