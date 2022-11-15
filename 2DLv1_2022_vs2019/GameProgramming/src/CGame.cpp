#include "CGame.h"
#include "CApplication.h"
#include "CBlock.h"
#include"CBlock2.h"
#include"CBlock3.h"
#include"CBlock4.h"
#include "CPlayer2.h"
#include "CEnemy2.h"
#include "CEnemy3.h"
#include "CPoint.h"
#include "CCamera.h"
#include "main.h"
#include"CGoal.h"
#include"CItem.h"
#include"CItem2.h"
#include"CItem3.h"

void CGame::Start()
{
	CameraSet();
	//ゲームの描画
	CApplication::CharacterManager()->Render();
    CCamera::End();
	//UI処理
	mpUi->Hp(CPlayer2::Hp());
	mpUi->Point(CPlayer2::Point());
	mpUi->Enemy(CEnemy2::Num());
	mpUi->Enemy(CEnemy3::Num());
	mpUi->Goal(CGoal::Goal());
	mpUi->Render();
	mpUi->Start();
}

bool CGame::IsOver()
{	//HPが0以下か判定結果を戻す
	return mTime>= 1001;
}

void CGame::Over()
{
	CameraSet();
	//ゲームの描画
	CApplication::CharacterManager()->Render();
	CCamera::End();
	//UI処理
	mpUi->Hp(CPlayer2::Hp());
	mpUi->Point(CPlayer2::Point());
	//mpUi->Enemy(CEnemy2::Num());
	mpUi->Render();
	mpUi->Over();
}

CGame::~CGame()
{
	//全てのインスタンス削除
	CApplication::CharacterManager()->AllDelete();
	//UIを生成している時
	if (mpUi != nullptr)
	{	//UIを削除し、初期化
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
	//ゲームの描画
	CApplication::CharacterManager()->Render();
	CCamera::End();
	//UI処理
	mpUi->Hp(CPlayer2::Hp());
	mpUi->Point(CPlayer2::Point());
	mpUi->Enemy(CEnemy2::Num());
	mpUi->Enemy(CEnemy3::Num());
	mpUi->Goal(CGoal::Goal());
	mpUi->Render();
	mpUi->Clear();
}

CGame::CGame()
	: mpUi(nullptr)
	,mpPlayer(0)
	//,mpGoal(0)
	, mTime(0)
	, mCdx(0)
	, mCdy(0)
{
	CEnemy2::Num(0);
	CGoal::Goal(0);
	mpUi = new CUi();
	//テクスチャの入力
	CApplication::Texture()->Load(TEXTURE);
	CApplication::Texture1()->Load(HITO);
	CApplication::Texture2()->Load(SURAIMU);
	CApplication::Texture3()->Load(KODOMO);
	CApplication::Texture4()->Load(GOAL);
	CApplication::Texture5()->Load(UGOKUBLOCK);
	CApplication::Texture6()->Load(AITEM);
	CApplication::Texture7()->Load(TEKI);

	//定数の定義
	const int ROWS = 20; //行数
	const int COLS = 25; //列数
	//2次元配列のマップ
	int map[ROWS][COLS] =
	{ 
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,0,0,1,1,0,0,1,0,3,0,0,1,0,0,0,1,0,3,0,1,0,0,0,1},
		{1,1,0,0,0,1,0,0,1,1,1,0,0,0,1,1,0,0,1,5,0,0,1,0,1},
		{1,1,1,1,0,1,1,0,0,1,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1},
		{1,1,0,0,0,0,0,1,0,0,0,1,1,1,0,0,1,1,0,0,1,0,0,0,1},
		{1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,1,1,0,1,0,0,1,1,0,1},
		{1,0,1,0,0,0,0,0,1,0,0,1,1,0,0,8,8,0,0,1,1,0,0,0,1},
		{1,1,0,0,1,1,1,1,0,0,0,0,10,0,1,9,9,0,0,0,0,5,1,0,1},
		{1,0,0,4,0,12,0,0,4,0,1,1,1,8,9,0,0,1,1,1,0,0,0,1,1},
		{1,0,1,1,0,0,0,0,1,1,1,2,8,10,10,1,0,0,0,0,1,1,0,5,1},
		{1,1,0,0,1,0,0,1,1,0,5,10,1,11,1,0,0,1,1,1,1,0,1,0,1},
		{1,0,0,1,0,1,0,1,0,0,1,10,0,1,0,1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1,0,10,0,0,0,0,1,0,1,1,1,0,1,1,1},
		{1,0,1,1,0,1,1,1,1,0,0,0,1,0,1,0,0,5,0,1,0,0,0,0,1},
		{1,0,1,0,0,0,0,1,0,0,1,1,0,7,0,1,1,1,0,0,0,0,0,1,1},
		{1,1,0,0,1,0,1,0,0,1,0,0,0,0,0,0,1,0,0,1,1,1,0,0,1},
		{1,6,0,1,1,0,0,0,1,0,0,0,1,0,0,1,0,0,0,0,0,0,0,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,11,1,1,1,1,1,1,1,1,1,1,1},
	};

	//マップの作成
	//行数分繰り返し
	for (int row = 0; row < ROWS; row++)
	{
		//列数分繰り返し
		for (int col = 0; col < COLS; col++)
		{
			//1の時、ブロック生成
			if (map[row][col] == 1)
			{
				//ブロックを生成して、キャラクタマネージャに追加
				CApplication::CharacterManager()->Add(
					new CBlock(TIPSIZE + TIPSIZE * 2 * col,
						TIPSIZE + TIPSIZE * 2 * row,
						TIPSIZE, TIPSIZE, CApplication::Texture()));
			}
			//2の時、プレイヤー生成
			if (map[row][col] == 2)
			{
				//カメラ用差分
				mCdx = WINDOW_WIDTH / 2 - (TIPSIZE + TIPSIZE * 2 * col);
				mCdy = WINDOW_HEIGHT / 2 - (TIPSIZE + TIPSIZE * 2 * row);
				//プレイヤーを生成して、キャラクタマネージャに追加
				CApplication::CharacterManager()->Add(
					mpPlayer = new CPlayer2(TIPSIZE + TIPSIZE * 2 * col,
						TIPSIZE + TIPSIZE * 2 * row,
						TIPSIZE, TIPSIZE, CApplication::Texture3()));
			}
			//3の時、敵生成
			if (map[row][col] == 3)
			{
				//敵を生成して、キャラクタマネージャに追加
				CApplication::CharacterManager()->Add(
					new CEnemy2(TIPSIZE + TIPSIZE * 2 * col,
						TIPSIZE + TIPSIZE * 2 * row,
						TIPSIZE, TIPSIZE, CApplication::Texture2()));
			}
			//4の時、折り返しポイント作成
			if (map[row][col] == 4)
			{
				//折り返しポイントを生成して、キャラクタマネージャに追加
				CApplication::CharacterManager()->Add(
					new CPoint(TIPSIZE + TIPSIZE * 2 * col,
						TIPSIZE + TIPSIZE * 2 * row,
						TIPSIZE, TIPSIZE, CCharacter::ETag::ETURN));
			}
			if (map[row][col] == 5)
			{
				//ブロックを生成して、キャラクタマネージャに追加
				CApplication::CharacterManager()->Add(
					new CBlock2(TIPSIZE + TIPSIZE * 2 * col,
						TIPSIZE + TIPSIZE * 2 * row,
						TIPSIZE, TIPSIZE, CApplication::Texture()));
			}
			if (map[row][col] == 6)
			{
				//ゴールを生成して、キャラクタマネージャに追加
				CApplication::CharacterManager()->Add(
					new CGoal(TIPSIZE + TIPSIZE * 2 * col,
						TIPSIZE + TIPSIZE * 2 * row,
						TIPSIZE, TIPSIZE, CApplication::Texture4()));
			}
			if (map[row][col] == 7)
			{
				//ブロックを生成して、キャラクタマネージャに追加
				CApplication::CharacterManager()->Add(
					new CBlock3(TIPSIZE + TIPSIZE * 2 * col,
						TIPSIZE + TIPSIZE * 2 * row,
						TIPSIZE, TIPSIZE, CApplication::Texture()));
			}
			if (map[row][col] == 8)
			{
				//アイテムの時計を生成して、キャラクタマネージャに追加
				CApplication::CharacterManager()->Add(
					new CItem(TIPSIZE + TIPSIZE * 2 * col,
						TIPSIZE + TIPSIZE * 2 * row,
						TIPSIZE, TIPSIZE, CApplication::Texture6()));
			}
			if (map[row][col] == 9)
			{
				//アイテムのスピードアップを生成して、キャラクタマネージャに追加
				CApplication::CharacterManager()->Add(
					new CItem2(TIPSIZE + TIPSIZE * 2 * col,
						TIPSIZE + TIPSIZE * 2 * row,
						TIPSIZE, TIPSIZE, CApplication::Texture6()));
			}
			if (map[row][col] == 10)
			{
				//アイテムのポイントを生成して、キャラクタマネージャに追加
				CApplication::CharacterManager()->Add(
					new CItem3(TIPSIZE + TIPSIZE * 2 * col,
						TIPSIZE + TIPSIZE * 2 * row,
						TIPSIZE, TIPSIZE, CApplication::Texture6()));
			}
			//11の時、ブロック生成
			if (map[row][col] == 11)
			{
				//ブロックを生成して、キャラクタマネージャに追加
				CApplication::CharacterManager()->Add(
					new CBlock4(TIPSIZE + TIPSIZE * 2 * col,
						TIPSIZE + TIPSIZE * 2 * row,
						TIPSIZE, TIPSIZE, CApplication::Texture()));
			}
			//12の時、敵生成
			if (map[row][col] == 12)
			{
				//敵を生成して、キャラクタマネージャに追加
				CApplication::CharacterManager()->Add(
					new CEnemy3(TIPSIZE + TIPSIZE * 2 * col,
						TIPSIZE + TIPSIZE * 2 * row,
						TIPSIZE, TIPSIZE, CApplication::Texture7()));
			}
		}
	}
}

void CGame::Update()
{
	//更新、衝突、削除、描画
	CApplication::CharacterManager()->Update();
	CApplication::CharacterManager()->Collision();
	CApplication::CharacterManager()->Delete();
	CameraSet();
	CApplication::CharacterManager()->Render();
	CCamera::End();
	//UI
	mpUi->Time(mTime++);
	//mpUi->Goal(mTime);
	mpUi->Hp(CPlayer2::Hp());
	mpUi->Point(CPlayer2::Point());
	mpUi->Enemy(CEnemy2::Num());
	mpUi->Goal(CGoal::Goal());
	mpUi->Render();
}

void CGame::CameraSet()
{
	float x = mpPlayer->X() + mCdx;
	float y = mpPlayer->Y() + mCdy;
	CCamera::Start(x - WINDOW_WIDTH / 2
		, x + WINDOW_WIDTH / 2
		, y - WINDOW_HEIGHT / 2
		, y + WINDOW_HEIGHT / 2
	);
}
