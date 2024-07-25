#include "CLockWall2.h"
#include "CImage.h"
#include "CPlayer.h"

// コンストラク
CLockWall2::CLockWall2(float x, float y)
	: mLevel(0)
{
	mpLockWall = new CImage("Wall");
	mpLockWall->SetPos(x, y);
	mpLockWall->SetSize(100.0f, 100.0f);
	mpLockWall->SetColor(1.0f, 1.0f, 1.0f, 0.5f);
}

// デストラクタ
CLockWall2::~CLockWall2()
{
}

// 更新
void CLockWall2::Update()
{
	CPlayer* player = CPlayer::Instance();
	mLevel = player->Status().level;
	if (mLevel >= 10)
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
