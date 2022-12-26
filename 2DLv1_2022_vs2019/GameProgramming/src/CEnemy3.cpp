#include "CEnemy3.h"
#include"CAttack.h"
#include "CApplication.h"
#include"stdio.h"
#include "stdlib.h"
#include"time.h"

#define TEXCOORD  1, 40, 53, 17	//テクスチャマッピング
#define TEXCRY 45, 80, 53, 17	//テクスチャマッピング
#define TEXCOORD2  40,85,53,17	//右向き2
#define TEXLEFT1  85, 40, 53, 17	//左向き1
#define TEXLEFT2  40, 1, 53, 17	//左向き2

int CEnemy3::sNum = 0;

void CEnemy3::Num(int num)
{
	sNum = num;
}

int CEnemy3::Num()
{
	return sNum;
}

void CEnemy3::Collision()
{
	CApplication::CharacterManager()->Collision(this);
}

void CEnemy3::Collision(CCharacter* m, CCharacter* o)
{
	//めり込み調整変数を宣言する
	float x, y;
	switch (o->Tag())
	{
	case ETag::ETURN:
		//折り返しに当たった時
		if (CRectangle::Collision(o, &x, &y))
		{
			//めり込まない位置まで戻す
			X(X() + x);
			Y(Y() + y);
			//X軸速度を反転させる
			mVx = -mVx;
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
				int item[3][3]//{ 0,0,0 };
				{
					item[0][0] = 0 > 10,
					item[1][1] = 10 > 20,
					item[2][2] = 20 > 30
				};
				i = (rand() % 100) + 1;
				if (i <= 0 || i >= 10) {
					double probability = 0.70; // 確率（70%）
					srand((unsigned)time(NULL)); // 乱数の初期化
					if ((double)rand() / RAND_MAX < probability) {
						//アイテムのポイントを生成して、キャラクタマネージャに追加
						item[0][0], CApplication::CharacterManager()->Add(
							new CItem3(X(),
								Y(),
								TIPSIZE, TIPSIZE, CApplication::Texture6()));
					}
					return;
				}
				if (i <= 10 || i >= 20) {
					double probability1 = 0.10; // 確率（10%）
					srand((unsigned)time(NULL)); // 乱数の初期化
					if ((double)rand() / RAND_MAX < probability1) {
						//アイテムのポイントを生成して、キャラクタマネージャに追加
						item[1][1], CApplication::CharacterManager()->Add(
							new CItem2(X(),
								Y(),
								TIPSIZE, TIPSIZE, CApplication::Texture6()));
					}
					return;
				}
				if (i <= 20 || i >= 30) {
					double probability2 = 0.10; // 確率（10%）
					srand((unsigned)time(NULL)); // 乱数の初期化
					if ((double)rand() / RAND_MAX < probability2) {
						//アイテムの時計を生成して、キャラクタマネージャに追加
						item[2][2], CApplication::CharacterManager()->Add(
							new CItem(X(),
								Y(),
								TIPSIZE, TIPSIZE, CApplication::Texture6()));
					}
					return;
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
		//break;
		//default:
		//	if (CRectangle::Collision(o))
		//	{
		//		//mState = EState::ESTOP;
		//		mEnabled = false;
		//	}
	}
}

//int CItem3::mItem3 = 0;
//int CItem2::mItem2 = 0;
//int CItem::mItem = 0;

CEnemy3::CEnemy3(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w, h);
	Texture(pt, TEXCOORD);
	mTag = ETag::EENEMY;
	//X軸速度の初期値を移動速度にする
	mVx = VELOCITY;
	//敵数に1加算する
	sNum++;
}

void CEnemy3::Update()
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
	case EState::EANNIHILATION:
		//泣く画像を設定
		//Texture(Texture(), TEXCRY);
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
				if (mState == EState::EMOVE)
				{
					//Texture(Texture(), TEXCRY);
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
			if (mVx < 0.0f) //左へ移動
			{
				//左向き2を設定
				Texture(Texture(), TEXLEFT2);
			}
			else
			{
				//2番目の画像を設定
				Texture(Texture(), TEXCOORD2);
				if (mState == EState::EMOVE)
				{
					//Texture(Texture(), TEXCRY);
				}
			}
		}
		break;
	}
}