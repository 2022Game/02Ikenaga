#include "CLockWall.h"
#include "CImage.h"
#include "CPlayer.h"

// �R���X�g���N
CLockWall::CLockWall(float x, float y)
	: mLevel(0)
{
	mpLockWall = new CImage("Wall");
	mpLockWall->SetPos(x, y);
	mpLockWall->SetSize(100.0f, 100.0f);
	mpLockWall->SetColor(1.0f, 1.0f, 1.0f, 0.5f);
}

// �f�X�g���N�^
CLockWall::~CLockWall()
{
}

// �X�V
void CLockWall::Update()
{
	CPlayer* player = CPlayer::Instance();
	mLevel = player->Status().level;
	if (mLevel >= 5)
	{
		mpLockWall->SetEnable(false);
		mpLockWall->SetShow(false);
	}
	else
	{
		mpLockWall->SetEnable(true);
		mpLockWall->SetShow(true);
	}
}
