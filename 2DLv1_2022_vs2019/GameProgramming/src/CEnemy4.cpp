#include "CEnemy4.h"
#include"CItem.h"
#include"CItem2.h"
#include"CItem3.h"
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