#include <stdio.h>
#include "CUi.h"

void CUi::Clear()
{
	mFont.Draw(36, 300, 18, 36, "GAME CLEAR!!");
	mFont.Draw(36, 200, 18, 36, "PUSH ENTER KEY!");
}

void CUi::Over()
{
	mFont.Draw(36, 300, 18, 36, "GAME OVER!!");
	mFont.Draw(36, 200, 18, 36, "PUSH ENTER KEY!");
}

void CUi::Start()
{
	mFont.Draw(36, 300, 18, 36, "START PUSH ENTER KEY!");
	//mFont1.Draw(90,118,10,30,"A");
}

void CUi::Goal(int goal)
{
	mGoal = goal;
}

void CUi::Enemy(int enemy)
{
	mEnemy = enemy;
}

CUi::CUi()
	: mHp(0)
	, mTime(0)
	,mPulltime(0)
	, mEnemy(0)
	,mGoal(0)
	,mPoint(0)
{
	mFont.Load("FontWhite.png", 1, 64);
	//mFont1.Load("image9-1-522x512.png", 1, 64);
}

void CUi::Point(int point)
{
	mPoint = point;
}

void CUi::Hp(int hp)
{
	mHp = hp;
}

void CUi::Time(int time)
{
	mTime = time;
}

void CUi::Pulltime(int pulltime)
{
	mPulltime = pulltime;
}

void CUi::Render()
{
	char str[16];
	sprintf(str, "TIME:%01d", mTime);
	mFont.Draw(20, 580, 10, 20, str);
	sprintf(str, "PULLTIME:%d", mPulltime);
	mFont.Draw(20, 550, 10, 20, str);
	/*sprintf(str, "HP:%d", mHp);
	mFont.Draw(20, 550, 10, 20, str);*/
	sprintf(str, "POINT:%d", mPoint);
	mFont.Draw(20, 520, 10, 20, str);
	/*sprintf(str, "ENEMY:%d", mEnemy);
	mFont.Draw(20, 580, 10, 20, str);*/
}
