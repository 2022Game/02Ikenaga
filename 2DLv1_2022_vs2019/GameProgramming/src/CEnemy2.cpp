#include "CEnemy2.h"
#include"CAttack.h"
#include "CApplication.h"
#include"stdio.h"
#include "stdlib.h"
#include"time.h"

#define TEXCOORD  145, 192, 143, 101	//�e�N�X�`���}�b�s���O
#define TEXCRY 50, 75, 143, 101	//�e�N�X�`���}�b�s���O
#define TEXCOORD2 240,289,143,101	//�E����2
#define TEXLEFT1 192, 145, 143, 101	//������1
#define TEXLEFT2  289,240,143,101	//������2

//int mItem(void)
//{
//	int i;
//
//	//  time�֐����g���������̎�̐ݒ�
//	srand((unsigned int)time(NULL));
//
//	for (i = 0; i < 10; i++)
//	{
//		printf("%d\n", rand());
//	}
//
//	return 0;
//}
//int main(void)
//{ 
//	char ch;
//	char item;
//    int  i;
//	srand((unsigned int)time(nullptr));
//	while (ch != 'Q') {
//		i = (rand() % 5) + 1;
//		printf(item[]);
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
			//-mVx <=mVx;
		}
		break;
	case ETag::EENEMY:
		break;
	case ETag::EPLAYER:
		if (CRectangle::Collision(o))
		{
			if (o->State() == EState::ECRY)
			{
				mState = EState::EMOVE;
				int mTime = CGame::mTime;
				if (mTime % 30 == 0)
				{
					mState = EState::ESTOP;
					return;
				}
				else if (mTime % 30 == 0)
				{
					mState = EState::ESTOP;
					return;
				}
				//if (mTime % 30 == 0)
				//{
				//	mState = EState::EMOVE;
				//	//return ;
				//}
			}
			if (o->State() == EState::EJUMP)
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
				if (mState != EState::ECRY)
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
	}
}

int CItem3::mItem3 = 0;
int CItem2::mItem2 = 0;
int CItem::mItem = 0;

CEnemy2::CEnemy2(float x, float y, float w, float h, CTexture* pt)
:mItem3(0),mItem2(0),mItem(0)
{
	Set(x, y, w, h);
	Texture(pt, TEXCOORD);
	mTag = ETag::EENEMY;
	//X�����x�̏����l���ړ����x�ɂ���
	mVx = VELOCITY;
	//�G����1���Z����
	sNum++;
	//srand((unsigned int)time(NULL));
}

void CEnemy2::Update()
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
		case EState::ECRY:
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
