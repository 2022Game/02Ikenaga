#pragma once
#include "CFont.h"
//#include"CCharacter.h"
/*
* CUi
* ユーザーインタフェースクラス
*/
class CUi // public CCharacter
{
public:
	//ゲームクリア表示
	void Clear();
	//ゲームオーバー表示
	void Over();
	//スタート表示
	void Start();
	//Enemy(敵の数)
	void Goal(int goal);
	void Enemy(int enemy);
	CUi();
	void Hp(int hp);
	void Point(int point);
	//Time(時間)
    void Time(int time);
	//void Pulltime(int pulltime);
	void Render();
    int mTime;	//経過時間
protected:
	bool mEnabled;
private:
	int mGoal;
	int mEnemy;	//敵の数
	int mHp;
	int mPoint;  //ポイント
   // int mTime;	//経過時間
	//int mPulltime;
	CFont mFont;
	//CFont mFont1;
};
