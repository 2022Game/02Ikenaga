#include"CEnemy5.h"
#include"CItem.h"
#include"CItem2.h"
#include"CItem3.h"
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
				int i{};
				int item[3]{ 0,0,0 };
				srand((unsigned)time(NULL)); // �����̏�����
				i = (rand() % 100) + 1;
				//srand((unsigned)time(NULL)); // �����̏�����
				if (i <= 0 || i >= 10) {
					double probability = 0.50; // �m���i50%�j
					if ((double)rand() / RAND_MAX < probability) {
						//�A�C�e���̃|�C���g�𐶐����āA�L�����N�^�}�l�[�W���ɒǉ�
						item[0], CApplication::CharacterManager()->Add(
							new CItem3(X(),
								Y(),
								TIPSIZE, TIPSIZE, CApplication::Texture6()));
					}
				}
				if (i <= 10 || i >= 20) {
					double probability1 = 0.10; // �m���i10%�j
					//srand((unsigned)time(NULL)); // �����̏�����
					if ((double)rand() / RAND_MAX < probability1) {
						//�A�C�e���̃|�C���g�𐶐����āA�L�����N�^�}�l�[�W���ɒǉ�
						item[1], CApplication::CharacterManager()->Add(
							new CItem2(X(),
								Y(),
								TIPSIZE, TIPSIZE, CApplication::Texture6()));
					}
				}
				if (i <= 20 || i >= 30) {
					double probability2 = 0.10; // �m���i10%�j
					//srand((unsigned)time(NULL)); // �����̏�����
					if ((double)rand() / RAND_MAX < probability2) {
						//�A�C�e���̎��v�𐶐����āA�L�����N�^�}�l�[�W���ɒǉ�
						item[2], CApplication::CharacterManager()->Add(
							new CItem(X(),
								Y(),
								TIPSIZE, TIPSIZE, CApplication::Texture6()));
					}
				}
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
				int i{};
				int item[3]{ 0,0,0 };
				srand((unsigned)time(NULL)); // �����̏�����
				i = (rand() % 100) + 1;
				//srand((unsigned)time(NULL)); // �����̏�����
				if (i <= 0 || i >= 10) {
					double probability = 0.50; // �m���i50%�j
					if ((double)rand() / RAND_MAX < probability) {
						//�A�C�e���̃|�C���g�𐶐����āA�L�����N�^�}�l�[�W���ɒǉ�
						item[0], CApplication::CharacterManager()->Add(
							new CItem3(X(),
								Y(),
								TIPSIZE, TIPSIZE, CApplication::Texture6()));
					}
				}
				if (i <= 10 || i >= 20) {
					double probability1 = 0.10; // �m���i10%�j
					//srand((unsigned)time(NULL)); // �����̏�����
					if ((double)rand() / RAND_MAX < probability1) {
						//�A�C�e���̃|�C���g�𐶐����āA�L�����N�^�}�l�[�W���ɒǉ�
						item[1], CApplication::CharacterManager()->Add(
							new CItem2(X(),
								Y(),
								TIPSIZE, TIPSIZE, CApplication::Texture6()));
					}
				}
				if (i <= 20 || i >= 30) {
					double probability2 = 0.10; // �m���i10%�j
					//srand((unsigned)time(NULL)); // �����̏�����
					if ((double)rand() / RAND_MAX < probability2) {
						//�A�C�e���̎��v�𐶐����āA�L�����N�^�}�l�[�W���ɒǉ�
						item[2], CApplication::CharacterManager()->Add(
							new CItem(X(),
								Y(),
								TIPSIZE, TIPSIZE, CApplication::Texture6()));
					}
				}
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
				int i{};
				int item[3]{ 0,0,0 };
				srand((unsigned)time(NULL)); // �����̏�����
				i = (rand() % 100) + 1;
				//srand((unsigned)time(NULL)); // �����̏�����
				if (i <= 0 || i >= 10) {
					double probability = 0.50; // �m���i50%�j
					if ((double)rand() / RAND_MAX < probability) {
						//�A�C�e���̃|�C���g�𐶐����āA�L�����N�^�}�l�[�W���ɒǉ�
						item[0], CApplication::CharacterManager()->Add(
							new CItem3(X(),
								Y(),
								TIPSIZE, TIPSIZE, CApplication::Texture6()));
					}
				}
				if (i <= 10 || i >= 20) {
					double probability1 = 0.10; // �m���i10%�j
					//srand((unsigned)time(NULL)); // �����̏�����
					if ((double)rand() / RAND_MAX < probability1) {
						//�A�C�e���̃|�C���g�𐶐����āA�L�����N�^�}�l�[�W���ɒǉ�
						item[1], CApplication::CharacterManager()->Add(
							new CItem2(X(),
								Y(),
								TIPSIZE, TIPSIZE, CApplication::Texture6()));
					}
				}
				if (i <= 20 || i >= 30) {
					double probability2 = 0.10; // �m���i10%�j
					//srand((unsigned)time(NULL)); // �����̏�����
					if ((double)rand() / RAND_MAX < probability2) {
						//�A�C�e���̎��v�𐶐����āA�L�����N�^�}�l�[�W���ɒǉ�
						item[2], CApplication::CharacterManager()->Add(
							new CItem(X(),
								Y(),
								TIPSIZE, TIPSIZE, CApplication::Texture6()));
					}
				}
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
	Texture(pt, TEXCOORDY);
	mTag = ETag::EENEMY;
	//X�����x�̏����l���ړ����x�ɂ���
	mVy = VELOCITY;
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