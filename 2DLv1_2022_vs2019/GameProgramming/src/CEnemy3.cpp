#include "CEnemy3.h"
#include"CAttack.h"
#include "CApplication.h"
#include"stdio.h"
#include "stdlib.h"
#include"time.h"

#define TEXCOORD  1, 40, 53, 17	//�e�N�X�`���}�b�s���O
#define TEXCRY 45, 80, 53, 17	//�e�N�X�`���}�b�s���O
#define TEXCOORD2  40,85,53,17	//�E����2
#define TEXLEFT1  85, 40, 53, 17	//������1
#define TEXLEFT2  40, 1, 53, 17	//������2

int CEnemy3::sNum = 0;

void CEnemy3::Num(int num)
{
	sNum = num;
}

int CEnemy3::Num()
{
	return sNum;
}

void CEnemy3::Collision()
{
	CApplication::CharacterManager()->Collision(this);
}

void CEnemy3::Collision(CCharacter* m, CCharacter* o)
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
		//break;
		//default:
		//	if (CRectangle::Collision(o))
		//	{
		//		//mState = EState::ESTOP;
		//		mEnabled = false;
		//	}
	}
}

//int CItem3::mItem3 = 0;
//int CItem2::mItem2 = 0;
//int CItem::mItem = 0;

CEnemy3::CEnemy3(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w, h);
	Texture(pt, TEXCOORD);
	mTag = ETag::EENEMY;
	//X�����x�̏����l���ړ����x�ɂ���
	mVx = VELOCITY;
	//�G����1���Z����
	sNum++;
}

void CEnemy3::Update()
{
	if (CItem3::mItem3 >= 10)
	{
		mEnabled = false;
	}
	if (CItem2::mItem2 >= 20)
	{
		mEnabled = false;
	}
	if (CItem::mItem >= 30)
	{
		mEnabled = false;
	}
	switch (mState)
	{
	case EState::EANNIHILATION:
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
				if (mState == EState::EMOVE)
				{
					//Texture(Texture(), TEXCRY);
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
			if (mVx < 0.0f) //���ֈړ�
			{
				//������2��ݒ�
				Texture(Texture(), TEXLEFT2);
			}
			else
			{
				//2�Ԗڂ̉摜��ݒ�
				Texture(Texture(), TEXCOORD2);
				if (mState == EState::EMOVE)
				{
					//Texture(Texture(), TEXCRY);
				}
			}
		}
		break;
	}
}