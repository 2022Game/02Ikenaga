#include "CLock2.h"
#include "CImage.h"
#include "CPlayer.h"

// �R���X�g���N
CLock2::CLock2(float x, float y)
	: mLevel(0)
{
	mpLock = new CImage("Lock");
	mpLock->SetPos(x, y);
	mpLock->SetSize(60.0f, 60.0f);
	mpLock->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
}

// �f�X�g���N�^
CLock2::~CLock2()
{
}

// �X�V
void CLock2::Update()
{
	CPlayer* player = CPlayer::Instance();
	mLevel = player->Status().level;
	if (mLevel >= 10)
	{
		mpLock->SetEnable(false);
		mpLock->SetShow(false);
	}
	else
	{
		mpLock->SetEnable(true);
		mpLock->SetShow(true);
	}
}