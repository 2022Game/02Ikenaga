#include"CEnemy5.h"
#include"CItem.h"
#include"CItem2.h"
#include"CItem3.h"
#include"CAttack.h"
#include "CApplication.h"
#include"stdio.h"
#include "stdlib.h"
#include"time.h"

#define TEXCOORD 147, 164, 97, 72	//テクスチャマッピング
#define TEXCOORDEX 172, 188, 97, 72
#define TEXCOORDY 195,213,97,72

int CEnemy5::sNum = 0;

void CEnemy5::Num(int num)
{
	sNum = num;
}

int CEnemy5::Num()
{
	return sNum;
}

void CEnemy5::Collision()
{
	CApplication::CharacterManager()->Collision(this);
}

void CEnemy5::Collision(CCharacter* m, CCharacter* o)
{
	//めり込み調整変数を宣言する
	float x, y;
	switch (o->Tag())
	{
	case ETag::EBLOCK4:
		//折り返しに当たった時
		if (CRectangle::Collision(o, &x, &y))
		{
			//めり込まない位置まで戻す
			X(X() + x);
			Y(Y() + y);
			//X軸速度を反転させる
			mVy = -mVy;
		}
		break;
	case ETag::EENEMY:
		break;
	case ETag::EPLAYER:
		if (CRectangle::Collision(o))
		{
			if (o->State() == EState::EJUMP)
			{
				//敵数1減算
				if (mState != EState::EANNIHILATION)
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
				if (mState != EState::EANNIHILATION)
				{
					//sNum--;
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
		//default:
		//	if (CRectangle::Collision(o))
		//	{
		//		//mState = EState::ESTOP;
		//		mEnabled = false;
		//	}
	}
}

CEnemy5::CEnemy5(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w, h);
	Texture(pt, TEXCOORDY);
	mTag = ETag::EENEMY;
	//X軸速度の初期値を移動速度にする
	mVy = VELOCITY;
	//敵数に1加算する
	sNum++;
}

void CEnemy5::Update()
{
	switch(mState)
	{
	case EState::ECRY:
		//泣く画像を設定
		//Texture(Texture(), TEXCRY);
		break;
	case EState::EMOVE:
		//X軸速度分、X座標を更新する
		Y(Y() + mVy);
		const int PITCH = 32;//画像を切り替える間隔
		if ((int)Y() % PITCH < PITCH / 2)
		{
			if (mVy < 0.0f) //左へ移動
			{
				//左向き１を設定
				Texture(Texture(), TEXCOORDEX);
				if (mState == EState::EMOVE)
				{
					Texture(Texture(), TEXCOORDY);
				}
			}
			else
			{
				//通常の画像を設定
				Texture(Texture(), TEXCOORD);
			}
		}
		else
		{
			if (mVy < 0.0f) //左へ移動
			{
				//左向き2を設定
				Texture(Texture(), TEXCOORD);
			}
			else
			{
				//2番目の画像を設定
				Texture(Texture(),TEXCOORDEX);
				if (mState == EState::EMOVE)
				{
					Texture(Texture(), TEXCOORDY);
				}
			}
		}
		break;
	}
}