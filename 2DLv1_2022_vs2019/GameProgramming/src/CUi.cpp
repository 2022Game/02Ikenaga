#include <stdio.h>
#include "CUi.h"
//#include "CCharacter.h"

//int CUi::mTime;

void CUi::Clear()
{
	mFont.Draw(36, 300, 18, 36, "GAME CLEAR!!");
	mFont.Draw(36, 200, 18, 36, "PUSH ENTER KEY!");
	char str[16];
	sprintf(str, "TIME:%01d", mTime);
	mFont.Draw(20, 580, 10, 23, str);
}

void CUi::Over()
{
	mFont.Draw(36, 300, 18, 36, "GAME OVER!!");
	mFont.Draw(36, 200, 18, 36, "PUSH ENTER KEY!");
}

void CUi::Start()
{
	mFont.Draw(30, 300, 15, 20, "START PUSH ENTER KEY!");
	mFont.Draw(36, 355, 20, 36, "RACE FOR TIME");
	mFont.Draw(20, 70, 8, 10, "PUSH J KEY TO JUMP");
	mFont.Draw(20, 50, 8, 10, "PUSH A KEY TO MOVE LEFT");
	mFont.Draw(20, 30, 8, 10, "PUSH D KEY TO MOVE RIGHT");
	mFont.Draw(20, 10, 8, 10, "PUSH SHIFT KEY TO ATTACK");
	//mFont1.Draw(90,118,10,30,"A");
}
//void CFont::Render()

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
	//,mPulltime(0)
	,mTime(0)
	, mEnemy(0)
	,mGoal(0)
	,mPoint(0)
{
	mFont.Load("FontWhite.png", 1, 64);
	//mFont1.Load("image9-1-522x512.png", 1, 64);
	//mTime = 0;
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

//void CUi::Pulltime(int pulltime)
//{
//	mPulltime = pulltime;
//}

void CUi::Render()
{
	char str[16];
	sprintf(str, "TIME:%01d", mTime);
	mFont.Draw(20, 580, 10, 23, str);
		//mTime = mEnabled = false;
	//sprintf(str, "PULLTIME:%d", mPulltime);
	//mFont.Draw(20, 550, 10, 20, str);
	/*sprintf(str, "HP:%d", mHp);
	mFont.Draw(20, 550, 10, 20, str);*/
	sprintf(str, "POINT:%d", mPoint);
	mFont.Draw(20, 550, 10, 10, str);
	/*sprintf(str, "ENEMY:%d", mEnemy);
	mFont.Draw(20, 580, 10, 20, str);*/
}
