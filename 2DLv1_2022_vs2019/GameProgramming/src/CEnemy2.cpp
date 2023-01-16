#include "CEnemy2.h"
#include"CAttack.h"
#include "CApplication.h"
#include"stdio.h"
#include "stdlib.h"
#include"time.h"
#include<stdbool.h>
#include <cstdio>

#define TEXCOORD  145, 192, 143, 101	//テクスチャマッピング
#define TEXCRY 50, 75, 143, 101	//テクスチャマッピング
#define TEXCOORD2 240,289,143,101	//右向き2
#define TEXLEFT1 192, 145, 143, 101	//左向き1
#define TEXLEFT2  289,240,143,101	//左向き2

//int mItem(void)
//{
//	int i;
//
//	//  time関数を使った乱数の種の設定
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
	//めり込み調整変数を宣言する
	float x, y;
	switch (o->Tag())
	{
	case ETag::EBLOCK:
		//折り返しに当たった時
		if (CRectangle::Collision(o, &x, &y))
		{
			//めり込まない位置まで戻す
			X(X() + x);
			Y(Y() + y);
			//X軸速度を反転させる
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
				//敵数1減算
				if (mState != EState::ECRY)
				{
					sNum--;
				}
				mEnabled = false;
				int i{};
				int item[3]{ 0,0,0 };
				srand((unsigned)time(NULL)); // 乱数の初期化
				i = (rand() % 100) + 1;
				//srand((unsigned)time(NULL)); // 乱数の初期化
				if (i <= 0 || i >= 10) {
					double probability = 0.50; // 確率（50%）
					if ((double)rand() / RAND_MAX < probability) {
						//アイテムのポイントを生成して、キャラクタマネージャに追加
						item[0], CApplication::CharacterManager()->Add(
							new CItem3(X(),
								Y(),
								TIPSIZE, TIPSIZE, CApplication::Texture6()));
					}
				}
				if (i <= 10 || i >= 20) {
					double probability1 = 0.10; // 確率（10%）
					//srand((unsigned)time(NULL)); // 乱数の初期化
					if ((double)rand() / RAND_MAX < probability1) {
						//アイテムのポイントを生成して、キャラクタマネージャに追加
						item[1], CApplication::CharacterManager()->Add(
							new CItem2(X(),
								Y(),
								TIPSIZE, TIPSIZE, CApplication::Texture6()));
					}
				}
				if (i <= 20 || i >= 30) {
					double probability2 = 0.10; // 確率（10%）
					//srand((unsigned)time(NULL)); // 乱数の初期化
					if ((double)rand() / RAND_MAX < probability2) {
						//アイテムの時計を生成して、キャラクタマネージャに追加
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
				//敵数1減算
				if (mState != EState::ECRY)
				{
					sNum--;
				}
				mEnabled = false;
				int i{};
				int item[3]{ 0,0,0 };
				srand((unsigned)time(NULL)); // 乱数の初期化
				i = (rand() % 100) + 1;
				//srand((unsigned)time(NULL)); // 乱数の初期化
				if (i <= 0 || i >= 10) {
					double probability = 0.50; // 確率（50%）
					if ((double)rand() / RAND_MAX < probability) {
						//アイテムのポイントを生成して、キャラクタマネージャに追加
						item[0], CApplication::CharacterManager()->Add(
							new CItem3(X(),
								Y(),
								TIPSIZE, TIPSIZE, CApplication::Texture6()));
					}
				}
				if (i <= 10 || i >= 20) {
					double probability1 = 0.10; // 確率（10%）
					//srand((unsigned)time(NULL)); // 乱数の初期化
					if ((double)rand() / RAND_MAX < probability1) {
						//アイテムのポイントを生成して、キャラクタマネージャに追加
						item[1], CApplication::CharacterManager()->Add(
							new CItem2(X(),
								Y(),
								TIPSIZE, TIPSIZE, CApplication::Texture6()));
					}
				}
				if (i <= 20 || i >= 30) {
					double probability2 = 0.10; // 確率（10%）
					//srand((unsigned)time(NULL)); // 乱数の初期化
					if ((double)rand() / RAND_MAX < probability2) {
						//アイテムの時計を生成して、キャラクタマネージャに追加
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
				//敵数1減算
				if (mState != EState::ECRY)
				{
					//sNum--;
				}
				mEnabled = false;
				//int i;
				////int r, n;
				////int mItem[3][3];
				////srand(10);
				//int rnd = rand() % 100;
				//srand((unsigned int)time(nullptr));
				//if (0 <= rnd && rnd < 30) {
				//	// 50%で時間が減るアイテム
				//	return CApplication::CharacterManager()->Add(
				//						new CItem(X(),
				//							Y(),
				//							TIPSIZE, TIPSIZE, CApplication::Texture6()));
				//}
				/*else if (CItem::mItem >=70 <= rnd && rnd < 100) {
					mEnabled = false;
				}*/
				//else if (50 <= rnd && rnd < 100) {
				//	// 10%でスピードアップするアイテム
				//	return CApplication::CharacterManager()->Add(
				//		new CItem2(X(),
				//			Y(),
				//			TIPSIZE, TIPSIZE, CApplication::Texture6()));
				//}
					/*if (20 <= rnd && rnd < 100) {
						mEnabled = false;
					}*/
				//}
				//else if (CItem3::mItem3 <= 0 <= rnd && rnd < 50)
				//{
				//	mEnabled = false;
				//}
				//else { //if(50 <= rnd && rnd < 100)
				//  // 50%でコインのアイテム
				//	return CApplication::CharacterManager()->Add(
				//						new CItem3(X(),
				//							Y(),
				//							TIPSIZE, TIPSIZE, CApplication::Texture6()));
				//}
				/*else if ((30 <= rnd && rnd < 100))
				{
					mEnabled = false;
				}*/

				srand((unsigned int)time(nullptr));
				bool d_flag = false;
				int num_pro_d = rand() % 100 + 1;
				int num_pro_m = 100 - num_pro_d;
				int num_pro_h = 100 - num_pro_d;
				int num_pro_v = 100 - num_pro_d;
				double tmp_random = (double)rand() / RAND_MAX;
				if (tmp_random < (double)num_pro_d / 100)
				{
					d_flag = true;
				}
				/*if (d_flag != false)
				{
					CItem3::mItem3 -= mEnabled = false;
				}*/
				else if ((rand() % 100) < 50){
					{
						CApplication::CharacterManager()->Add(
							new CItem3(X(),
								Y(),
								TIPSIZE, TIPSIZE, CApplication::Texture6()));
					}
				}
				/*if (d_flag != false)
				{
					CItem2::mItem2 -=mEnabled = false;
				}*/
				else if ((rand() % 100) < 25){
					{
						CApplication::CharacterManager()->Add(
							new CItem2(X(),
								Y(),
								TIPSIZE, TIPSIZE, CApplication::Texture6()));
					}
				}
				 /* if (d_flag != false)
				{
					  CItem::mItem  -= mEnabled = false;
				}*/
				else
				{
					if ((rand() % 100) < 25)
					{
						CApplication::CharacterManager()->Add(
							new CItem(X(),
								Y(),
								TIPSIZE, TIPSIZE, CApplication::Texture6()));
					}
				}
				return ;
				//srand((unsigned int)time(nullptr));
				//int Random;
				//for ( i = 0; i < 10; i++)
				//{
				//	//乱数値の表示
				//	int mItem3(0 + rand() % 80);
				//		double probability = 0.80;
				//		if ((double)rand() / RAND_MAX < probability) {
				//			//アイテムのポイントを生成して、キャラクタマネージャに追加
				//				CApplication::CharacterManager()->Add(
				//				new CItem3(X(),
				//					Y(),
				//					TIPSIZE, TIPSIZE, CApplication::Texture6()));
				//		}
				//	srand((unsigned int)time(nullptr));
				//	int mItem2(0 + rand() % 80);
				//	double probability2 = 0.80;
				//	if ((double)rand() / RAND_MAX < probability2)
				//	{
				//		CApplication::CharacterManager()->Add(
				//						new CItem2(X(),
				//							Y(),
				//							TIPSIZE, TIPSIZE, CApplication::Texture6()));
				//				}

				//	//printf("%lf\n", rand() / (double)RAND_MAX);
				//	return ;
				//}//

				//return;
			//	int i;
			//	int i{};
			//	int item[3]{ 0,0,0 };
			//	srand((unsigned)time(NULL)); // 乱数の初期化
			//	i = (rand() % 100) + 1;
			//	//srand((unsigned)time(NULL)); // 乱数の初期化
			//	if (i <= 0 || i >= 10) {
			//		double probability = 0.50; // 確率（50%）
			//		if ((double)rand() / RAND_MAX < probability) {
			//			//アイテムのポイントを生成して、キャラクタマネージャに追加
			//			item[0], CApplication::CharacterManager()->Add(
			//				new CItem3(X(),
			//					Y(),
			//					TIPSIZE, TIPSIZE, CApplication::Texture6()));
			//		}
			//	}
			//	if (i <= 10 || i >= 20) {
			//		double probability1 = 0.10; // 確率（10%）
			//		//srand((unsigned)time(NULL)); // 乱数の初期化
			//		if ((double)rand() / RAND_MAX < probability1) {
			//			//アイテムのポイントを生成して、キャラクタマネージャに追加
			//			item[1], CApplication::CharacterManager()->Add(
			//				new CItem2(X(),
			//					Y(),
			//					TIPSIZE, TIPSIZE, CApplication::Texture6()));
			//		}
			//	}
			//	if (i <= 20 || i >= 30) {
			//		double probability2 = 0.10; // 確率（10%）
			//		//srand((unsigned)time(NULL)); // 乱数の初期化
			//		if ((double)rand() / RAND_MAX < probability2) {
			//			//アイテムの時計を生成して、キャラクタマネージャに追加
			//			item[2], CApplication::CharacterManager()->Add(
			//				new CItem(X(),
			//					Y(),
			//					TIPSIZE, TIPSIZE, CApplication::Texture6()));
			//		}
				//}
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
	//X軸速度の初期値を移動速度にする
	mVx = VELOCITY;
	//敵数に1加算する
	sNum++;
	//srand((unsigned int)time(NULL));
}

void CEnemy2::Update()
{
	//bool d_flag = false;
	if (CItem3::mItem3 > (rand() % 100))
	{
		mEnabled = false;
	}
	if (CItem2::mItem2 > (rand() % 100))
	{
		mEnabled = false;
	}
	if (CItem::mItem > (rand() % 100))
	{
		mEnabled = false;
	}
	switch (mState)
	{
		case EState::ECRY:
		break;
	case EState::EMOVE:
		//X軸速度分、X座標を更新する
		X(X() + mVx);
		const int PITCH = 32;//画像を切り替える間隔
		if ((int)X() % PITCH < PITCH / 2)
		{
			if (mVx < 0.0f) //左へ移動
			{
				//左向き１を設定
				Texture(Texture(), TEXLEFT1);
			}
			else
			{
				//通常の画像を設定
				Texture(Texture(), TEXCOORD);
			}
		}
		else
		{
			if (mVx < 0.0f) //左へ移動
			{
				//左向き2を設定
				Texture(Texture(), TEXLEFT2);
			}
			else
			{
				//2番目の画像を設定
				Texture(Texture(), TEXCOORD2);
			}
		}
		break;
	}
} 

//int arr() {
//	int arr[10];
//	int arr1[] = { 1, 1, 2, 3, 5, 8, 13, 21, 34, 55 };
//	int arr2[10] = { 1, 1, 2, 3, 5, 8, 13, 21, 34, 55 };
//	arr[0] = 1;
//	arr[1] = 3;
//	printf("%d\n", arr[0]);
//	return 0;
//}

//int arr() {
//
//	int arr1[] = { 1, 1, 2, 3, 5, 8, 13, 21, 34, 55 };
//
//	int arr2[10] = { 1, 1, 2, 3, 5, 8, 13, 21, 34, 55 };
//
//	int arr3[10] = { 1, 1, 2, 3, 5, 8 };
//	printf("%d", arr3[1]);  // 0
//
//	return 0;
//}