#include "CPlayer2.h"
#include "CApplication.h"
#include"CAttack.h"
#include"CAttack2.h"

#define TEXCOORD 30, 66, 100, 50	//�e�N�X�`���}�b�s���O
#define TEXCRY 196, 216, 158, 128	//�e�N�X�`���}�b�s���O
#define GRAVITY (TIPSIZE / 16.0f)	//�d�͉����x16
#define JUMPV0 (TIPSIZE / 1.5f)//�W�����v�̏���
#define JUMPVO2 (TIPSIZE / 1.4f)
#define TEXCOORD2 150,189,340,293   //�E����2
#define TEXCOORD3 510,550,340,293   //�E����
#define TEXCOORD4 750,790,340,293   //�E�����W�����v
#define TEXCOORD5 790,750,340,293    //�������W�����v
#define TEXLEFT1 190,150,340,293	//�E����2
#define TEXLEFT2 550,510,340,293	//������2
#define TEXSLEEPY  860,925,226,190   //���x��
#define TEXPOINT 200,200,100,55
#define TEXATTACK 640,710,100,55   //�U�����[�V����
#define TEXATTACK2 710,640,100,55   //���΍U�����[�V����
#define VELOCITY 2.0f	//�ړ����x
#define VELOCITY2 4.0f
#define SOKUDO 1.0f
#define HP 3 //HP�̏����l��3
#define POINT 0
#define SE_JUMP "res\\jump.wav" //�W�����v�̉����t�@�C��
#define ATTACK1  860,960,225,180   //�U���̃e�N�X�`��

int CPlayer2::mPulltime=0;

int  CPlayer2::Pulltime()
{
	return mPulltime;
}


int CPlayer2::sPoint = 0;	//POINT

int  CPlayer2::Point()
{
	return sPoint;
}

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
					if (mInvincible == 0)
					{
						mInvincible = 100;
						mState = EState::ESTOP;
						//sHp--;
					}
				}
			}
			else
			{	//�W�����v�łȂ���΋���
				mState = EState::ECRY;
				if (mInvincible == 0)
				{
					mInvincible = 100;
					mState = EState::ESTOP;
					//sHp--;
				}
			}
		}
		break;
	case ETag::EITEM:
		if (CRectangle::Collision(o, &x, &y))
		{
			//���v�̏Փ˔�������s
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
					mState = EState::EDOWN;
					if (mdown == 0)
					{
						mdown = 1;
						mPulltime=mPulltime-100;
					}
				}
			}
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
					mState = EState::EDOWN;
					if (mdown == 0)
					{
						mdown = 1;
						mPulltime = mPulltime - 100;
					}
				}
			}
			else
			{	//�W�����v�łȂ���΋���
				mState = EState::EDOWN;
				if (mdown == 0)
				{
					mdown = 1;
					mPulltime = mPulltime - 100;
				}
			}
		}
				break;
	case ETag::EITEM2:
		if (CRectangle::Collision(o, &x, &y))
		{
			//�C�̏Փ˔�������s
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
					mState = EState::EUP;
					if (mUp == 0)
					{
						mUp = 100;
					}
				}
			}
			else
				{	//�W�����v�łȂ���΋���
					mState = EState::EUP;
					if (mUp == 0)
					{
						mUp = 100;
					}
				}
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
					mState = EState::EUP;
					if (mUp == 0)
					{
						mUp = 100;
						//mVx = VELOCITY2;
					}
				}
			}
			else
			{	//�W�����v�łȂ���΋���
				mState = EState::EUP;
				if (mUp == 0)
				{
					mUp = 100;
					//mVx = VELOCITY2;
					//sHp--;
				}
			}
		}
		case ETag::EITEM3:
			if (CRectangle::Collision(o, &x, &y))
			{
				//�R�C���̏Փ˔�������s
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
						mState = EState::EHIT;
						if (mnothing == 0)
						{
							mnothing = 1;
							sPoint++;
						}
					}
				}
				else
				{	//�W�����v�łȂ���΋���
					mState = EState::EHIT;
					if (mnothing == 0)
					{
						mnothing = 1;
						sPoint++;
						//sHp--;
					}
				}
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
						mState = EState::EHIT;
						if (mnothing == 0)
						{
							mnothing = 1;
							sPoint++;
						}
					}
				}
				else
				{	//�W�����v�łȂ���΋���
					mState = EState::EHIT;
					if (mnothing == 0)
					{
						mnothing = 1;
						sPoint++;
						//sHp--;
					}
				}
			}
			break;
	case ETag::EGOAL:
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
					//mState = EState::ESTOP;
					mState = EState::EMOVE;
				}
			}
			if (x != 0.0f)
			{
				//Y�����x��0�ɂ���
				mVx = 0.0f;
				if (x > 0.0f)
				{
					//mState = EState::ESTOP;
					mState = EState::EMOVE;
				}
			}
		}
			break;
	case ETag::EPLAYER:
		break;
	case ETag::EBLOCK:
	case ETag::EBLOCK3:
	case ETag::EBLOCK4:
	//jcase ETag::EITEM:
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
	: mInvincible(0), mVy(0.0f),mVx(0.0f)
	,mnothing(0),mUp(0),mdown(0)
{
	Set(x, y, w, h);
	Texture(pt, TEXSLEEPY);
	mTag = ETag::EPLAYER;
	sHp = HP;
	sPoint = POINT;
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
		if (mnothing > 0)
	{
		mnothing--;
	}
		if (mUp > 0)
		{
			mUp--;
		}
		if (mdown > 0)
		{
			mdown--;
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
	if (mState == EState::EATTACK)
	{
		if (mInput.Key(VK_SHIFT))
		{
			CApplication::CharacterManager()->Add(
				new CAttack(X(), Y() + H() - 20.0f
					, 30.0f, 20.0f, 106, 20, 50, 92, CApplication::Texture3()));
		}
	}
	if (mInput.Key('A'))
	{
		mVx = -VELOCITY;
		//		float x = X() - 4.0f;
		X(X() + mVx);
		if (mState == EState::EUP)
		{
			mVx = -VELOCITY2;
			X(X() + mVx);
		}
	}
	if (mInput.Key('D'))
	{
		mVx = VELOCITY;
		//		float x = X() - 4.0f;
		X(X() + mVx);
		if (mState == EState::EUP)
		{
			mVx = VELOCITY2;
			X(X() + mVx);
		}
	}
	//Y���W��Y�����x��������
	Y(Y() + mVy);
	//Y�����x�ɏd�͂����Z����
	mVy -= GRAVITY;
	if (mState == EState::EHIT)
	{
		if (mnothing == 0)
		{
			mnothing = 1;
			//sPoint++;
		}
	}
	if (mnothing > 0)
	{
		//mState = EState::EHIT;
		Texture(Texture(), TEXPOINT);
	}
		if (mState == EState::EUP)
		{
			Texture(Texture(), TEXPOINT);
			if (mUp == 0)
			{
				mUp = 100;
			}
		}
		if (mUp > 0)
		{
			mState = EState::EUP;
			Texture(Texture(), TEXPOINT);
		}

		if (mState == EState::EDOWN)
		{
			Texture(Texture(), TEXPOINT);
			if (mdown == 0)
			{
				mdown = 1;
				//mPulltime = mPulltime - 100;
			}
		}
		if (mUp > 0)
		{
			mState = EState::EUP;
			Texture(Texture(), TEXPOINT);
		}
		if (mState == EState::ECRY)
	{
		//�����摜��ݒ�
		Texture(Texture(), TEXSLEEPY);
		if (mInvincible == 0)
		{
			mInvincible = 100;
			mState = EState::ESTOP;
			//sHp--;
		}
	}
	if (mInvincible > 0)
	{
		Texture(Texture(), TEXSLEEPY);
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
					if (mState == EState::EJUMP)
					{
						Texture(Texture(), TEXCOORD5);
					}
					if (mInput.Key(VK_SHIFT))
					{
						CApplication::CharacterManager()->Add(
							new CAttack2(X(), Y() + H() - 20.0f
								, 30.0f, 20.0f, 106, 20, 50, 92, CApplication::Texture3()));
						Texture(Texture(), TEXATTACK2);
					}
			}
			else
			{
				//�ʏ�̉摜��ݒ�
				Texture(Texture(), TEXCOORD3);
		          if(mState == EState::EJUMP)
				{
					Texture(Texture(), TEXCOORD4);
				}
				  if (mInput.Key(VK_SHIFT))
				  {
					  CApplication::CharacterManager()->Add(
						  new CAttack(X(), Y() + H() - 20.0f
							  , 30.0f, 20.0f, 106, 20, 50, 92, CApplication::Texture3()));
					  Texture(Texture(), TEXATTACK);
				  }
			}
		}
		else
		{
			if (mVx < 0.0f) //���ֈړ�
			{
				//������2��ݒ�
				Texture(Texture(), TEXLEFT2);
				if (mState == EState::EJUMP)
				{
					Texture(Texture(), TEXCOORD5);
				}
				if (mInput.Key(VK_SHIFT))
				{
					CApplication::CharacterManager()->Add(
						new CAttack2(X(), Y() + H() - 20.0f
							, 30.0f, 20.0f, 106, 20, 50, 92, CApplication::Texture3()));
					Texture(Texture(), TEXATTACK2);
				}
			}
			else
			{
				//2�Ԗڂ̉摜��ݒ�
				Texture(Texture(), TEXCOORD2);
				if (mState == EState::EJUMP)
				{
					Texture(Texture(), TEXCOORD4);
				}
					if (mInput.Key(VK_SHIFT))
					{
						CApplication::CharacterManager()->Add(
							new CAttack(X(), Y() + H() - 20.0f
								, 30.0f, 20.0f, 106, 20, 50, 92, CApplication::Texture3()));
						Texture(Texture(), TEXATTACK);
					}
			}
		}

	}
}

