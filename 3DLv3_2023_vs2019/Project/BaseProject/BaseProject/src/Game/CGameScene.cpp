#include "CGameScene.h"
#include "CSceneManager.h"
#include "CField.h"
#include "CSky.h"
#include "CNightSky.h"
#include "CPlayer.h"
#include "CSword.h"
#include "CShield.h"
#include "CEnemy.h"
#include "CSlime.h"
#include "CSlime2.h"
#include "CSlime3.h"
#include "CMushroom.h"
#include "CMushroom2.h"
#include "CMushroom3.h"
#include "CTurtle.h"
#include "CTurtle2.h"
#include "CTurtle3.h"
#include "CCactus.h"
#include "CCactus2.h"
#include "CCactus3.h"
#include "CBee.h"
#include "CBee2.h"
#include "CBee3.h"
#include "CRay.h"
#include "CRay2.h"
#include "CRay3.h"
#include "CBeholder.h"
#include "CChest.h"
#include "CChest2.h"
#include "CChest3.h"
#include "CBoxer.h"
#include "CBoxer2.h"
#include "CLich.h"
#include "CDragon.h"
#include "CPortionRed.h"
#include "CPortionGreen.h"
#include "CPortionBlue.h"
#include "CCamera.h"
#include "CGameCamera.h"
#include "CGameCamera2.h"
#include "CInput.h"
#include "CGameMenu.h"
#include "CBGMManager.h"
#include "CLineEffect.h"
#include "CMenu.h"
#include "CKeyM.h"
#include "CKeyQ.h"
#include "CKeyE.h"
#include "CBuffSkill.h"
#include "CSlashSkill.h"
#include "CLockWall.h"
#include "CEnemyManager.h"
#include "Maths.h"

// コンストラクタ
CGameScene::CGameScene()
	: CSceneBase(EScene::eGame)
{
	CEnemyManager::Create();
}

// デストラクタ
CGameScene::~CGameScene()
{
	CGameMenu::ClearInstance();

	CEnemyManager::Delete();
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
	CResourceManager::Load<CModel>("Sky", "Field\\Sky\\Sky.obj");
	CResourceManager::Load<CModel>("NightSky", "Field\\NightSky\\NightSky.obj");

	// プレイヤー関連
	CResourceManager::Load<CModelX>("Player", "Character\\Player\\Dog.x");

	// エネミー関連
	CResourceManager::Load<CModelX>("Slime","Character\\Enemy\\Slime\\Slime.x");
	CResourceManager::Load<CModelX>("Slime2", "Character\\Enemy\\Slime\\SlimeOrange.x");
	//CResourceManager::Load<CModelX>("Slime3", "Character\\Enemy\\Slime\\SlimeBlue.x");
	CResourceManager::Load<CModelX>("Mushroom", "Character\\Enemy\\Mushroom\\Mushroom.x");
	CResourceManager::Load<CModelX>("Turtle", "Character\\Enemy\\Turtle\\Turtle.x");
	CResourceManager::Load<CModelX>("Ray", "Character\\Enemy\\Ray\\Ray.x");
	CResourceManager::Load<CModelX>("Bee", "Character\\Enemy\\Bee\\Bee.x");
	CResourceManager::Load<CModelX>("Cactus", "Character\\Enemy\\Cactus\\Cactus.x");
	CResourceManager::Load<CModelX>("Chest", "Character\\Enemy\\Chest\\Chest.x");
	CResourceManager::Load<CModelX>("Beholder", "Character\\Enemy\\Beholder\\Beholder.x");
	CResourceManager::Load<CModelX>("Boxer", "Character\\Enemy\\Boxer\\Boxer.x");
	//CResourceManager::Load<CModelX>("Boxer2", "Character\\Enemy\\Boxer\\Boxer2.x");
	CResourceManager::Load<CModelX>("Lich", "Character\\Enemy\\Lich\\Lich.x");

	// ボス関連
	CResourceManager::Load<CModelX>("Dragon", "Character\\Enemy\\Dragon\\Dragon.x");

	// 武器関連
	CResourceManager::Load<CModel>("Sword", "Item\\Equipment\\Sword\\Sword.obj");
	CResourceManager::Load<CModel>("Shield", "Item\\Equipment\\Shield\\Shield.obj");
	CResourceManager::Load<CModel>("ShieldRotate", "Item\\Equipment\\Shield\\ShieldRotate.obj");

	// アイテム関連
	CResourceManager::Load<CModel>("Portion", "Item\\Portion\\PortionRed.obj");
	CResourceManager::Load<CModel>("Portion2", "Item\\Portion\\PortionGreen.obj");
	CResourceManager::Load<CModel>("Portion3", "Item\\Portion\\PortionBlue.obj");

	// UI関連
	CResourceManager::Load<CTexture>("Frame", "Character\\Player\\HP\\Frame.png");
	CResourceManager::Load<CTexture>("Gauge", "UI\\white.png");
	CResourceManager::Load<CTexture>("FrameEdge","Character\\Player\\HP\\FrameEdge.png");
	CResourceManager::Load<CTexture>("Menu", "UI\\menu.png");
	CResourceManager::Load<CTexture>("KeyM", "UI\\Key\\mKey.png");
	CResourceManager::Load<CTexture>("KeyQ", "UI\\Key\\qKey.png");
	CResourceManager::Load<CTexture>("KeyE", "UI\\Key\\eKey.png");
	CResourceManager::Load<CTexture>("BuffSkill", "UI\\buffSkill.png");
	CResourceManager::Load<CTexture>("Wall", "UI\\black.png");

	// エフェクト関連
	CResourceManager::Load<CTexture>("Laser", "Effect\\laser.png");
	CResourceManager::Load<CModel>("Slash", "Effect\\Slash\\Slash.obj");
	CResourceManager::Load<CModel>("Needle", "Effect\\Needle\\Needle.obj");
	CResourceManager::Load<CModel>("Coin", "Effect\\Coin\\Coin.obj");
	CResourceManager::Load<CModel>("Wave", "Effect\\Wave\\Wave.obj");
	CResourceManager::Load<CModel>("Roar", "Effect\\Roar.obj");
	CResourceManager::Load<CModel>("Tornado", "Effect\\Tornado\\Tornado.obj");
	CResourceManager::Load<CModel>("MagicCircle", "Effect\\MagicCircle\\MagicCircle.obj");
	CResourceManager::Load<CModel>("HealCircle", "Effect\\Circle\\Circle.obj");
	CResourceManager::Load<CModel>("BuffCircle", "Effect\\Circle2\\Circle2.obj");
	CResourceManager::Load<CTexture>("Current", "Effect\\Current\\lightning_bolt.png");

	// BGM関連
	CResourceManager::Load<CSound>("SlashSound", "Sound\\SE\\slash.wav");

	// スライム関連の効果音
	CResourceManager::Load<CSound>("SlimeRun", "Sound\\EnemySE\\Slime\\SlimeRun1.wav");
	CResourceManager::Load<CSound>("SlimeAttack", "Sound\\EnemySE\\Slime\\SlimeAttack.wav");
	CResourceManager::Load<CSound>("SlimeAttack2", "Sound\\EnemySE\\Slime\\SlimeAttack2.wav");
	CResourceManager::Load<CSound>("SlimeDizzy", "Sound\\EnemySE\\Slime\\SlimeDizzy.wav");
	CResourceManager::Load<CSound>("SlimeHit", "Sound\\EnemySE\\Slime\\SlimeHit.wav");
	CResourceManager::Load<CSound>("SlimeDie", "Sound\\EnemySE\\Slime\\SlimeDie.wav");

	// ゲームBGMを読み込み
	CBGMManager::Instance()->Play(EBGMType::eGame);

	CField* field = new CField();
	CField* field2 = new CField();
	field2->Position(0.0f, 0.0f, -1000.0f);
	CField* field3 = new CField();
	field3->Position(0.0f, 0.0f, 1000.0f);

	// 空
	CSky* sky = new CSky();
	sky->Scale(200.0f, 200.0f, 200.0f);
	sky->Position(0.0f, -1.5f, 0.0f);

	// 夜空
	CNightSky* nightSky = new CNightSky();
	nightSky->Scale(200.0f, 200.0f, 200.0f);
	nightSky->Position(0.0f, -1.5f, 0.0f);

	CPlayer* player = new CPlayer();

	/*CCamera* mainCamera = new CCamera
	(
		DEFAULT_CAMERA_POS,
		player->Position()
	);
	mainCamera->SetFollowTargetTf(player);*/

	// CGameCamera2のテスト
	CVector atPos = player->Position() + CVector(DEFAULT_CAMERA_POS);
	CGameCamera2* mainCamera = new CGameCamera2
	(
		atPos + CVector(DEFAULT_CAMERA_POS),
		player->Position() + CVector(0.0f, 20.0f, 0.0f)
	);
	mainCamera->SetFollowTargetTf(player);
	mainCamera->AddCollider(field->GetCollider());
	mainCamera->AddCollider(field2->GetCollider());
	mainCamera->AddCollider(field3->GetCollider());
	mainCamera->SetHitColliderRatio(0.98f);
	player->Position(0.0f, 100.0f, 0.0f);

	// レッドスライム
	CSlime* enemy = new CSlime();
	enemy->Position(0.0f, 0.0f, -40.0f);

	// オレンジスライム
	CSlime2* enemy2 = new CSlime2();
	enemy2->Position(-40.0f, 0.0f, -40.0f);
	enemy2->Scale(25.0f, 25.0f, 25.0f);

	//// ブルースライム
	//CSlime3* enemy3 = new CSlime3();
	//enemy3->Position(-20.0f, 0.0f, -90.0f);
	//enemy3->Scale(35.0f, 35.0f, 35.0f);

	// マッシュルーム
	CMushroom* enemy4 = new CMushroom();
	enemy4->Position(150.0f, 0.0f, -150.0f);

	//// マッシュルーム2
	//CMushroom2* enemy5 = new CMushroom2();
	//enemy5->Position(210.0f, 0.0f, -150.0f);
	//enemy5->Scale(25.0f, 25.0f, 25.0f);

	//// マッシュルーム3
	//CMushroom3* enemy6 = new CMushroom3();
	//enemy6->Position(180.0f, 0.0f, -210.0f);
	//enemy6->Scale(35.0f, 35.0f, 35.0f);

	// 亀
	CTurtle* enemy7 = new CTurtle();
	enemy7->Position(0.0f, -0.2f, -300.0f);

	//// 亀2
	//CTurtle2* enemy8 = new CTurtle2();
	//enemy8->Position(-60.0f, -0.2f, -300.0f);
	//enemy8->Scale(25.0f, 25.0f, 25.0f);

	//// 亀3
	//CTurtle3* enemy9 = new CTurtle3();
	//enemy9->Position(-30.0f, -0.2f, -360.0f);
	//enemy9->Scale(35.0f, 35.0f, 35.0f);

	// エイ
	CRay* enemy10 = new CRay();
	enemy10->Position(150.0f, 0.0f, -450.0f);

	//// エイ2
	//CRay2* enemy11 = new CRay2();
	//enemy11->Position(210.0f, 0.0f, -450.0f);
	//enemy11->Scale(35.0f, 35.0f, 35.0f);

	//// エイ3
	//CRay3* enemy12 = new CRay3();
	//enemy12->Position(180.f,0.0f,-520.0f);
	//enemy12->Scale(45.0f, 45.0f, 45.0f);

	// 蜂
	CBee* enemy13 = new CBee();
	enemy13->Position(0.0f, 0.0f, -600.0f);

	//// 蜂2
	//CBee2* enemy14 = new CBee2();
	//enemy14->Position(-60.0f, 0.0f, -600.0f);
	//enemy14->Scale(30.0f, 30.0f, 30.0f);

	//// 蜂3
	//CBee3* enemy15 = new CBee3();
	//enemy15->Position(-30.0f, 0.0f, -660.0f);
	//enemy15->Scale(40.0f, 40.0f, 40.0f);

	// サボテン
	CCactus* enemy16 = new CCactus();
	enemy16->Position(150.0f, 0.0f, -750.0f);

	//// サボテン2
	//CCactus2* enemy17 = new CCactus2();
	//enemy17->Position(210.0f, 0.0f, -750.0f);
	//enemy17->Scale(30.0f, 30.0f, 30.0f);

	//// サボテン3
	//CCactus3* enemy18 = new CCactus3();
	//enemy18->Position(180.0f, 0.0f, -810.0f);
	//enemy18->Scale(40.0f, 40.0f, 40.0f);

	// チェストモンスター
	CChest* enemy19 = new CChest();
	enemy19->Position(0.0f, 0.0f, -900.0f);

	//// チェストモンスター2
	//CChest2* enemy20 = new CChest2();
	//enemy20->Position(-60.0f, 0.0f, -900.0f);
	//enemy20->Scale(30.0f, 30.0f, 30.0f);

	//// チェストモンスター3
	//CChest3* enemy21 = new CChest3();
	//enemy21->Position(-30.0f, 0.0f, -960.0f);
	//enemy21->Scale(30.0f, 30.0f, 30.0f);

	// 球体のモンスター
	CBeholder* enemy22 = new CBeholder();
	enemy22->Position(150.0f, 0.0f, -1050.0f);

	// ボクサー
	CBoxer* enemy25 = new CBoxer();
	enemy25->Position(0.0f, 0.0f, -1200.0f);

	//// ボクサー2
	//CBoxer2* enemy27 = new CBoxer2();
	//enemy27->Position(370.0f, 0.0f, 20.0f);
	//enemy27->Scale(15.0f, 15.0f, 15.0f);

	// リッチ
	CLich* enemy28 = new CLich();
	enemy28->Position(150.0f, 3.0f, -1350.0f);
	//enemy28->Position(0.0f, 3.0f, -200.0f);
	//enemy28->Rotate(0.0f,90.0f,-70.0f);

	// ドラゴン
	CDragon* bossEnemy = new CDragon();
	bossEnemy->Position(0.0f, 1.0f, -1500.0f);

	// メニューアイコンを作成
	CMenu* menu = new CMenu(10.0f, 680.0f);
	//Mキーを作成
	CKeyM* keyM = new CKeyM(40.0f, 670.0f);
	// Qキーを作成
	CKeyQ* keyQ = new CKeyQ(1012.0f, 560.0f);
	// Eキーを作成
	CKeyE* keyE = new CKeyE(1142.0f, 560.0f);
	// バフスキルを作成
	CBuffSkill* buhhSkill = new CBuffSkill(990.0f, 619.0f);
	// スラッシュスキル作成
	CSlashSkill* slashSkill = new CSlashSkill(1120.0f, 619.0f);
	// ロック中の壁
	CLockWall* lockWall = new CLockWall(990.0f, 619.0f);

	// ゲームメニューを作成
	CGameMenu::Instance();
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
	// ゲームメニューを開いてなければ、[Ｍ]キーでメニューを開く
	CGameMenu* menu = CGameMenu::Instance();
	if (!menu->IsOpened())
	{
		if (CInput::PushKey('M'))
		{
			menu->Open();
		}
	}
}
