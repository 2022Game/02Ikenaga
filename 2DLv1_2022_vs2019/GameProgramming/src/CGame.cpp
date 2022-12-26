#include "CGame.h"
#include "CApplication.h"
#include "CBlock.h"
#include"CBlock2.h"
#include"CBlock3.h"
#include"CBlock4.h"
#include"CBlock5.h"
#include "CPlayer2.h"
#include "CEnemy2.h"
#include "CEnemy3.h"
#include"CEnemy4.h"
#include"CEnemy5.h"
#include"CEnemy6.h"
#include"CEnemy7.h"
#include "CPoint.h"
#include "CCamera.h"
#include "main.h"
#include"CGoal.h"
#include"CGoal2.h"
#include"CGoal3.h"
//#include"CItem.h"
#include"CItem2.h"
#include"CItem3.h"

int CGame::mTime;

void CGame::Start()
{
	CameraSet();
	//�Q�[���̕`��
	CApplication::CharacterManager()->Render();
    CCamera::End();
	//UI����
	mpUi->Hp(CPlayer2::Hp());
	mpUi->Point(CPlayer2::Point());
	//mpUi->Pulltime(CPlayer2::Pulltime());
	mpUi->Enemy(CEnemy2::Num());
	mpUi->Enemy(CEnemy3::Num());
	mpUi->Goal(CGoal::Goal());
	mpUi->Render();
	mpUi->Start();
}

bool CGame::IsOver()
{	//HP��0�ȉ������茋�ʂ�߂�
	return mTime>= 6001;
}

void CGame::Over()
{
	CameraSet();
	//�Q�[���̕`��
	CApplication::CharacterManager()->Render();
	CCamera::End();
	//UI����
	mpUi->Hp(CPlayer2::Hp());
	mpUi->Point(CPlayer2::Point());
	//mpUi->Pulltime(CPlayer2::Pulltime());
	//mpUi->Enemy(CEnemy2::Num());
	mpUi->Render();
	mpUi->Over();
}

CGame::~CGame()
{
	//�S�ẴC���X�^���X�폜
	CApplication::CharacterManager()->AllDelete();
	//UI�𐶐����Ă��鎞
	if (mpUi != nullptr)
	{	//UI���폜���A������
		delete mpUi;
		mpUi = nullptr;
	}
}

bool CGame::IsClear()
{
	return CGoal::Goal() <=0;
}

void CGame::Clear()
{
	CameraSet();
	//�Q�[���̕`��
	CApplication::CharacterManager()->Render();
	CCamera::End();
	//UI����
	mpUi->Hp(CPlayer2::Hp());
	mpUi->Point(CPlayer2::Point());
	//mpUi->Pulltime(CPlayer2::Pulltime());
	mpUi->Enemy(CEnemy2::Num());
	mpUi->Enemy(CEnemy3::Num());
	mpUi->Goal(CGoal::Goal());
	mpUi->Render();
	mpUi->Clear();
}

CGame::CGame()
	: mpUi(nullptr)
	,mpPlayer(0)
	//,mpBlock(0)
	//,mpGoal(0)
	//,mTime(0)
	,mpItem(0)
	, mCdx(0)
	, mCdy(0)
{
	mTime = 0;
	CEnemy2::Num(0);
	CGoal::Goal(0);
	mpUi = new CUi();
	//�e�N�X�`���̓���
	CApplication::Texture()->Load(TEXTURE);
	CApplication::Texture1()->Load(HITO);
	CApplication::Texture2()->Load(SURAIMU);
	CApplication::Texture3()->Load(KODOMO);
	CApplication::Texture4()->Load(GOAL);
	CApplication::Texture5()->Load(UGOKUBLOCK);
	CApplication::Texture6()->Load(AITEM);
	CApplication::Texture7()->Load(TEKI);
	CApplication::Texture8()->Load(TEKI2);
	CApplication::Texture9()->Load(UPUP);
	CApplication::Texture10()->Load(TEKI3);
	CApplication::Texture11()->Load(TEKI4);

	//�萔�̒�`
	const int ROWS = 20; //�s��
	const int COLS = 25; //��
	//2�����z��̃}�b�v
	int map[ROWS][COLS] =
	{ 
		{1,1,1,1,11,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,13,0,0,1,0,0,1,3,0,0,0,1,0,0,13,1,0,3,0,1,0,9,13,1},
		{1,0,0,0,0,1,0,0,5,1,11,0,0,0,1,1,0,0,1,5,0,0,1,11,1},
		{1,8,1,1,7,1,1,0,0,1,15,0,0,0,0,0,0,0,0,1,1,0,0,1,1},
		{1,1,0,0,0,0,1,1,0,0,0,1,1,11,0,0,1,1,0,0,1,0,0,0,1},
		{1,18,0,1,11,0,0,0,1,1,0,0,10,0,0,1,1,0,1,0,0,1,1,7,1},
		{1,0,1,0,0,0,0,0,1,0,0,1,1,16,0,0,0,0,0,1,1,0,0,0,1},
		{1,1,0,0,1,1,1,1,0,0,0,0,0,0,1,0,1,0,0,0,0,5,1,0,1},
		{1,0,0,4,0,12,0,0,4,0,11,1,1,0,0,0,0,1,1,1,0,0,0,11,1},
		{1,1,1,11,0,0,1,0,1,1,1,2,9,0,0,1,4,0,12,0,4,11,0,13,1},
		{1,13,0,0,1,0,0,1,1,0,5,0,1,11,1,0,0,1,1,1,1,0,1,1,1},
		{1,10,1,7,0,1,0,0,0,0,1,0,0,1,0,1,4,0,0,12,0,4,0,13,1},
		{1,1,0,0,0,0,0,1,0,1,0,0,0,0,0,0,1,0,1,1,1,0,11,1,1},
		{1,0,0,11,1,1,1,1,1,0,0,0,1,0,1,0,0,5,0,1,0,7,0,0,1},
		{1,0,0,7,14,0,0,1,0,0,1,1,0,7,0,1,1,1,0,0,0,0,0,0,1},
		{1,1,1,0,0,0,1,0,0,1,17,0,0,0,0,13,1,0,0,1,1,11,7,1,1},
		{1,18,0,0,1,5,0,0,1,18,0,0,1,0,0,1,0,0,0,0,0,0,0,6,1},
		{1,1,1,11,1,1,1,1,1,1,1,1,1,11,1,1,1,1,1,1,1,1,11,1,1},
	};

	//�}�b�v�̍쐬
	//�s�����J��Ԃ�
	for (int row = 0; row < ROWS; row++)
	{
		//�񐔕��J��Ԃ�
		for (int col = 0; col < COLS; col++)
		{
			//1�̎��A�u���b�N����
			if (map[row][col] == 1)
			{
				//�u���b�N�𐶐����āA�L�����N�^�}�l�[�W���ɒǉ�
				CApplication::CharacterManager()->Add(
					new CBlock(TIPSIZE + TIPSIZE * 2 * col,
						TIPSIZE + TIPSIZE * 2 * row,
						TIPSIZE, TIPSIZE, CApplication::Texture()));
			}
			//2�̎��A�v���C���[����
			if (map[row][col] == 2)
			{
				//�J�����p����
				mCdx = WINDOW_WIDTH / 2 - (TIPSIZE + TIPSIZE * 2 * col);
				mCdy = WINDOW_HEIGHT / 2 - (TIPSIZE + TIPSIZE * 2 * row);
				//�v���C���[�𐶐����āA�L�����N�^�}�l�[�W���ɒǉ�
				CApplication::CharacterManager()->Add(
					mpPlayer = new CPlayer2(TIPSIZE + TIPSIZE * 2 * col,
						TIPSIZE + TIPSIZE * 2 * row,
						TIPSIZE, TIPSIZE, CApplication::Texture3()));
			}
			//3�̎��A�G����
			if (map[row][col] == 3)
			{
				//�G�𐶐����āA�L�����N�^�}�l�[�W���ɒǉ�
				CApplication::CharacterManager()->Add(
					new CEnemy2(TIPSIZE + TIPSIZE * 2 * col,
						TIPSIZE + TIPSIZE * 2 * row,
						TIPSIZE, TIPSIZE, CApplication::Texture2()));
			}
			//4�̎��A�܂�Ԃ��|�C���g�쐬
			if (map[row][col] == 4)
			{
				//�܂�Ԃ��|�C���g�𐶐����āA�L�����N�^�}�l�[�W���ɒǉ�
				CApplication::CharacterManager()->Add(
					new CPoint(TIPSIZE + TIPSIZE * 2 * col,
						TIPSIZE + TIPSIZE * 2 * row,
						TIPSIZE, TIPSIZE, CCharacter::ETag::ETURN));
			}
			if (map[row][col] == 5)
			{
				//�u���b�N�𐶐����āA�L�����N�^�}�l�[�W���ɒǉ�
				CApplication::CharacterManager()->Add(
					new CBlock2(TIPSIZE + TIPSIZE * 2 * col,
						TIPSIZE + TIPSIZE * 2 * row,
						TIPSIZE, TIPSIZE, CApplication::Texture()));
			}
			if (map[row][col] == 6)
			{
				//�S�[���𐶐����āA�L�����N�^�}�l�[�W���ɒǉ�
				CApplication::CharacterManager()->Add(
					new CGoal(TIPSIZE + TIPSIZE * 2 * col,
						TIPSIZE + TIPSIZE * 2 * row,
						TIPSIZE, TIPSIZE, CApplication::Texture4()));
			}
			if (map[row][col] == 7)
			{
				//�u���b�N�𐶐����āA�L�����N�^�}�l�[�W���ɒǉ�
				CApplication::CharacterManager()->Add(
					new CBlock3(TIPSIZE + TIPSIZE * 2 * col,
						TIPSIZE + TIPSIZE * 2 * row,
						TIPSIZE, TIPSIZE, CApplication::Texture()));
			}
			if (map[row][col] == 8)
			{
				//�A�C�e���̎��v�𐶐����āA�L�����N�^�}�l�[�W���ɒǉ�
				CApplication::CharacterManager()->Add(
					new CItem(TIPSIZE + TIPSIZE * 2 * col,
						TIPSIZE + TIPSIZE * 2 * row,
						TIPSIZE, TIPSIZE, CApplication::Texture6()));
			}
			if (map[row][col] == 9)
			{
				//�A�C�e���̃X�s�[�h�A�b�v�𐶐����āA�L�����N�^�}�l�[�W���ɒǉ�
				CApplication::CharacterManager()->Add(
					new CItem2(TIPSIZE + TIPSIZE * 2 * col,
						TIPSIZE + TIPSIZE * 2 * row,
						TIPSIZE, TIPSIZE, CApplication::Texture6()));
			}
			if (map[row][col] == 10)
			{
				//�A�C�e���̃|�C���g�𐶐����āA�L�����N�^�}�l�[�W���ɒǉ�
				CApplication::CharacterManager()->Add(
					new CItem3(TIPSIZE + TIPSIZE * 2 * col,
						TIPSIZE + TIPSIZE * 2 * row,
						TIPSIZE, TIPSIZE, CApplication::Texture6()));
			}
			//11�̎��A�u���b�N����
			if (map[row][col] == 11)
			{
				//�u���b�N�𐶐����āA�L�����N�^�}�l�[�W���ɒǉ�
				CApplication::CharacterManager()->Add(
					new CBlock4(TIPSIZE + TIPSIZE * 2 * col,
						TIPSIZE + TIPSIZE * 2 * row,
						TIPSIZE, TIPSIZE, CApplication::Texture()));
			}
			//12�̎��A�G����
			if (map[row][col] == 12)
			{
				//�G�𐶐����āA�L�����N�^�}�l�[�W���ɒǉ�
				CApplication::CharacterManager()->Add(
					new CEnemy3(TIPSIZE + TIPSIZE * 2 * col,
						TIPSIZE + TIPSIZE * 2 * row,
						TIPSIZE, TIPSIZE, CApplication::Texture7()));
			}
			//13�̎��A�S�[������
			if (map[row][col] == 13)
			{
				//�S�[���𐶐����āA�L�����N�^�}�l�[�W���ɒǉ�
				CApplication::CharacterManager()->Add(
					new CGoal2(TIPSIZE + TIPSIZE * 2 * col,
						TIPSIZE + TIPSIZE * 2 * row,
						TIPSIZE, TIPSIZE, CApplication::Texture4()));
			}
			//14�̎��A�G����
			if (map[row][col] == 14)
			{
				//�G�𐶐����āA�L�����N�^�}�l�[�W���ɒǉ�
				CApplication::CharacterManager()->Add(
					new CEnemy4(TIPSIZE + TIPSIZE * 2 * col,
						TIPSIZE + TIPSIZE * 2 * row,
						TIPSIZE, TIPSIZE, CApplication::Texture8()));
			}
			//15�̎��A�G����
			if (map[row][col] == 15)
			{
				//�G�𐶐����āA�L�����N�^�}�l�[�W���ɒǉ�
				CApplication::CharacterManager()->Add(
					new CEnemy5(TIPSIZE + TIPSIZE * 2 * col,
						TIPSIZE + TIPSIZE * 2 * row,
						TIPSIZE, TIPSIZE, CApplication::Texture10()));
			}
			//16�̎��A�G����
			if (map[row][col] == 16)
			{
				//�G�𐶐����āA�L�����N�^�}�l�[�W���ɒǉ�
				CApplication::CharacterManager()->Add(
					new CEnemy6(TIPSIZE + TIPSIZE * 2 * col,
						TIPSIZE + TIPSIZE * 2 * row,
						TIPSIZE, TIPSIZE, CApplication::Texture10()));
			}
			//17�̎��A�G����
			if (map[row][col] == 17)
			{
				//�u���b�N�𐶐����āA�L�����N�^�}�l�[�W���ɒǉ�
				CApplication::CharacterManager()->Add(
					new CBlock5(TIPSIZE + TIPSIZE * 2 * col,
						TIPSIZE + TIPSIZE * 2 * row,
						TIPSIZE, TIPSIZE, CApplication::Texture()));
			}
			//18�̎��A�S�[������
			if (map[row][col] == 18)
			{
				//�S�[���𐶐����āA�L�����N�^�}�l�[�W���ɒǉ�
				CApplication::CharacterManager()->Add(
					new CGoal3(TIPSIZE + TIPSIZE * 2 * col,
						TIPSIZE + TIPSIZE * 2 * row,
						TIPSIZE, TIPSIZE, CApplication::Texture4()));
			}
			//19�̎��A�G����
			if (map[row][col] == 19)
			{
				//�G�𐶐����āA�L�����N�^�}�l�[�W���ɒǉ�
				CApplication::CharacterManager()->Add(
					new CEnemy7(TIPSIZE + TIPSIZE * 2 * col,
						TIPSIZE + TIPSIZE * 2 * row,
						TIPSIZE, TIPSIZE, CApplication::Texture11()));
			}
		}
	}
}

void CGame::Update()
{
	//�X�V�A�ՓˁA�폜�A�`��
	CApplication::CharacterManager()->Update();
	CApplication::CharacterManager()->Collision();
	CApplication::CharacterManager()->Delete();
	CameraSet();
	CApplication::CharacterManager()->Render();
	CCamera::End();
	//UI
	mpUi->Time(mTime++);
	mpUi->Hp(CPlayer2::Hp());
	mpUi->Point(CPlayer2::Point());
	//mpUi->Pulltime(CPlayer2::Pulltime());
	mpUi->Enemy(CEnemy2::Num());
	mpUi->Goal(CGoal::Goal());
	mpUi->Render();
}

void CGame::CameraSet()
{
	float x = mpPlayer->X() + mCdx;
	float y = mpPlayer->Y() + mCdy;
	CCamera::Start(x - WINDOW_WIDTH / 2
		, x+ WINDOW_WIDTH / 2
		, y - WINDOW_HEIGHT / 2
		, y + WINDOW_HEIGHT / 2
	);
}