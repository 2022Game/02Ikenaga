#include "CGameScene.h"
#include "CSceneManager.h"
#include "CField.h"
#include "CPlayer.h"
#include "CSword.h"
#include "CShield.h"
#include "CSlime.h"
#include "CSlime2.h"
#include "CSlime3.h"
#include "CMushroom.h"
#include "CMushroom2.h"
#include "CMushroom3.h"
#include "CTurtle.h"
#include "CRich.h"
#include "CDragon.h"
#include "CPortionRed.h"
#include "CPortionGreen.h"
#include "CPortionBlue.h"
#include "CCane.h"
#include "CCamera.h"
#include "CGameCamera.h"
#include "CGameCamera2.h"
#include "CInput.h"
#include "CGameMenu.h"
#include "CBGMManager.h"
//#include "CLineEffect.h"

// コンストラクタ
CGameScene::CGameScene()
	: CSceneBase(EScene::eGame)
	,SlimeReTime(0)
	,Slime2ReTime(0)
	,Slime3ReTime(0)
	,MushroomReTime(0)
	,Mushroom2ReTime(0)
	,Mushroom3ReTime(0)
	,mpGameMenu(nullptr)
{
}

// デストラクタ
CGameScene::~CGameScene()
{
}

// シーン読み込み
void CGameScene::Load()
{
	// 背景色設定
	System::SetClearColor(0.1921569f, 0.3019608f, 0.4745098f, 1.0f);

	//ここでゲーム中に必要な
	//リソースの読み込みやクラスの生成を行う

	// フィールド関連
	CResourceManager::Load<CModel>("Field", "Field\\field.obj");

	// プレイヤー関連
	CResourceManager::Load<CModelX>("Player", "Character\\Player\\Dog.x");
	CResourceManager::Load<CModelX>("Player2", "Character\\Rich.x");

	// エネミー関連
	CResourceManager::Load<CModelX>("Slime","Character\\Enemy\\Slime\\Slime.x");
	CResourceManager::Load<CModelX>("Slime2", "Character\\Enemy\\Slime\\SlimeOrange.x");
	CResourceManager::Load<CModelX>("Slime3", "Character\\Enemy\\Slime\\SlimeBlue.x");
	CResourceManager::Load<CModelX>("Mushroom", "Character\\Enemy\\Mushroom\\Mushroom.x");
	CResourceManager::Load<CModelX>("Turtle", "Character\\Enemy\\Turtle\\Turtle.x");

	// ボス関連
	CResourceManager::Load<CModelX>("Dragon", "Character\\Enemy\\Dragon\\Dragon.x");

	// 武器関連
	CResourceManager::Load<CModel>("Sword", "Item\\Equipment\\Sword\\Sword.obj");
	CResourceManager::Load<CModel>("Shield", "Item\\Equipment\\Shield\\Shield.obj");
	CResourceManager::Load<CModel>("Cane", "Character\\Cane.obj");

	// アイテム関連
	CResourceManager::Load<CModel>("Portion", "Item\\Portion\\PortionRed.obj");
	CResourceManager::Load<CModel>("Portion2", "Item\\Portion\\PortionGreen.obj");
	CResourceManager::Load<CModel>("Portion3", "Item\\Portion\\PortionBlue.obj");

	// UI関連
	CResourceManager::Load<CTexture>("HpFrame", "Character\\Player\\HP\\Frame.png");
	CResourceManager::Load<CTexture>("HpGauge", "UI\\white.png");
	CResourceManager::Load<CTexture>("SaFrame", "Character\\Player\\HP\\Frame.png");
	CResourceManager::Load<CTexture>("SaGauge", "UI\\white.png");
	CResourceManager::Load<CTexture>("AvoidanceFrame", "Character\\Player\\HP\\Frame.png");
	CResourceManager::Load<CTexture>("AvoidanceGaugeGauge", "UI\\white.png");
	CResourceManager::Load<CTexture>("FrameEdge","Character\\Player\\HP\\FrameEdge.png");

	// エフェクト関連
	CResourceManager::Load<CTexture>("Laser", "Effect\\laser.png");

	// BGM関連
	CResourceManager::Load<CSound>("SlashSound", "Sound\\SE\\slash.wav");

	// その他
	//CResourceManager::Load<CImage>("Exp", "Item\\exp.png");

	// ゲームBGMを読み込み
	CBGMManager::Instance()->Play(EBGMType::eGame);

	new CField();

	CPlayer* player = new CPlayer();

	CCamera* mainCamera = new CCamera
	(
		DEFAULT_CAMERA_POS,
		player->Position()
	);
	mainCamera->SetFollowTargetTf(player);

	player->Position(0.0f, 20.0f, 0.0f);

	// レッドスライム
	CSlime* enemy = new CSlime();
	enemy->Position(0.0f, 0.0f, -40.0f);
	enemy->Scale(25.0f, 25.0f, 25.0f);

	// オレンジスライム
	CSlime2* enemy2 = new CSlime2();
	enemy2->Position(-40.0f, 0.0f, -40.0f);
	enemy2->Scale(25.0f, 25.0f, 25.0f);

	// ブルースライム
	CSlime3* enemy3 = new CSlime3();
	enemy3->Position(-20.0f, 0.0f, -90.0f);
	enemy3->Scale(35.0f, 35.0f, 35.0f);

	// マッシュルーム
	CMushroom* enemy4 = new CMushroom();
	enemy4->Position(150.0f, 0.0f, -150.0f);
	enemy4->Scale(25.0f, 25.0f, 25.0f);

	// マッシュルーム2
	CMushroom2* enemy5 = new CMushroom2();
	enemy5->Position(210.0f, 0.0f, -150.0f);
	enemy5->Scale(25.0f, 25.0f, 25.0f);

	// マッシュルーム3
	CMushroom3* enemy6 = new CMushroom3();
	enemy6->Position(180.0f, 0.0f, -210.0f);
	enemy6->Scale(35.0f, 35.0f, 35.0f);

	// 亀
	CTurtle* enemy7 = new CTurtle();
	enemy7->Position(100.0f, -0.2f, 20.0f);
	enemy7->Scale(25.0f, 25.0f, 25.0f);

	CRich* enemy8 = new CRich();
	enemy8->Position(100.0f, 5.0f, 50.0f);
	enemy8->Rotate(0.0f,90.0f,-70.0f);
	enemy8->Scale(0.1f, 0.1f, 0.1f);

	// ドラゴン
	CDragon* bossEnemy = new CDragon();
	bossEnemy->Position(-80.0f, 20.0f, -400.0f);
	bossEnemy->Scale(7.0f, 7.0f, 7.0f);

	CPortionRed* portionred = new CPortionRed();
	portionred->Position(90.0f, 10.0f, 0.0f);
	portionred->Scale(70.0f, 70.0f, 70.0f);

	CPortionGreen* portiongreen = new CPortionGreen();
	portiongreen->Position(80.0f, 10.0f, 0.0f);
	portiongreen->Scale(70.0f, 70.0f, 70.0f);

	CPortionBlue* portionblue = new CPortionBlue();
	portionblue->Position(70.0f, 10.0f, 0.0f);
	portionblue->Scale(70.0f, 70.0f, 70.0f);

	// ゲームメニューを作成
	mpGameMenu = new CGameMenu();
}

// ゲームクリア判定
bool CGameScene::IsClear()
{
	return mpPlayer->mHp >= 10000;
}

// ゲームクリア処理
void CGameScene::Clear()
{
	//CSceneManager::ClearInstance();
}

// シーンの更新処理
void CGameScene::Update()
{
	// BGM再生中でなければ、BGMを再生
	//if (!mpGameBGM->IsPlaying())
	//{
	//	mpGameBGM->PlayLoop(-1, 1.0f, false, 1.0f);
	//}

	// ゲームメニューを開いてなければ、[Ｍ]キーでメニューを開く
	if (!mpGameMenu->IsOpened())
	{
		if (CInput::PushKey('M'))
		{
			mpGameMenu->Open();
		}
	}

	// プレイヤーのHPが0以下になったら
	if (mpPlayer->mHp <= 0)
	{
		CSceneManager::Instance()->LoadScene(EScene::eGameOver);
	}

	// レッドスライムの復活
	if (mpSlime->mHp <= 0)
	{
		(SlimeReTime++/100);
		if (SlimeReTime > 1000)
		{
			// レッドスライム
			CSlime* enemy = new CSlime();
			enemy->Position(0.0f, 0.0f, -40.0f);
			enemy->Scale(25.0f, 25.0f, 25.0f);
			SlimeReTime = 0;
		}
	}

	// オレンジスライムの復活
	if (mpSlime2->mHp <= 0)
	{
		(Slime2ReTime++ / 100);
		if (Slime2ReTime > 1000)
		{
			// オレンジスライム
			CSlime2* enemy2 = new CSlime2();
			enemy2->Position(-40.0f, 0.0f, -40.0f);
			enemy2->Scale(25.0f, 25.0f, 25.0f);
			Slime2ReTime = 0;
		}
	}

	//	ブルースライムの復活
	if (mpSlime3->mHp <= 0)
	{
		(Slime3ReTime++ / 100);
		if (Slime3ReTime > 1000)
		{
			// ブルースライム
			CSlime3* enemy3 = new CSlime3();
			enemy3->Position(-20.0f, 0.0f, -90.0f);
			enemy3->Scale(35.0f, 35.0f, 35.0f);
			Slime3ReTime = 0;
		}
	}

	// マッシュルームの復活
	if (mpMushroom->mHp <= 0)
	{
		(MushroomReTime++ / 100);
		if (MushroomReTime > 1000)
		{
			// マッシュルーム
			CMushroom* enemy4 = new CMushroom();
			enemy4->Position(150.0f, 0.0f, -150.0f);
			enemy4->Scale(25.0f, 25.0f, 25.0f);
			MushroomReTime = 0;
		}
	}

	// マッシュルーム2の復活
	if (mpMushroom2->mHp <= 0)
	{
		(Mushroom2ReTime++ / 100);
		if (Mushroom2ReTime > 1000)
		{
			// マッシュルーム2
			CMushroom2* enemy5 = new CMushroom2();
			enemy5->Position(210.0f, 0.0f, -150.0f);
			enemy5->Scale(25.0f, 25.0f, 25.0f);
			Mushroom2ReTime = 0;
		}
	}

	// マッシュルーム3の復活
	if (mpMushroom3->mHp <= 0)
	{
		(Mushroom3ReTime++ / 100);
		if (Mushroom3ReTime > 1000)
		{
			// マッシュルーム3
			CMushroom3* enemy6 = new CMushroom3();
			enemy6->Position(180.0f, 0.0f, -210.0f);
			enemy6->Scale(35.0f, 35.0f, 35.0f);
			Mushroom3ReTime = 0;
		}
	}
}
