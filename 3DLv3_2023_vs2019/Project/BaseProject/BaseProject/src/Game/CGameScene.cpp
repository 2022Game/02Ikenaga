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
#include "CTurtle.h"
#include "CCactus.h"
#include "CBee.h"
#include "CRay.h"
#include "CBeholder.h"
#include "CChest.h"
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
#include "CGameOverScene.h"
#include "CBGMManager.h"
#include "CLineEffect.h"
#include "CMenu.h"
#include "CKeyM.h"
#include "CKeyQ.h"
#include "CKeyE.h"
#include "CBuffSkill.h"
#include "CSlashSkill.h"
#include "CLockWall.h"
#include "CLockWall2.h"
#include "CEnemyManager.h"
#include "CLock.h"
#include "CLock2.h"
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
	CInput::ShowCursor(false);

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
	//CResourceManager::Load<CModelX>("Slime2", "Character\\Enemy\\Slime\\SlimeOrange.x");
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
	CResourceManager::Load<CTexture>("SlashSkill", "UI\\slashSkill.png");
	CResourceManager::Load<CTexture>("Wall", "UI\\black.png");
	CResourceManager::Load<CTexture>("Lock", "UI\\Lock.png");

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

	// バフスキルを作成
	CBuffSkill* buhhSkill = new CBuffSkill(990.0f, 619.0f);
	// スラッシュスキル作成
	CSlashSkill* slashSkill = new CSlashSkill(1120.0f, 619.0f);
	// ロック中の壁
	CLockWall* lockWall = new CLockWall(990.0f, 619.0f);
	// ロック中の壁2
	CLockWall2* lockWall2 = new CLockWall2(1120.0f, 619.0f);
	// ロック
	CLock* lock = new CLock(1010.0f, 630.0f);
	// ロック
	CLock2* lock2 = new CLock2(1140.0f, 630.0f);
	// Qキーを作成
	CKeyQ* keyQ = new CKeyQ(1014.0f, 560.0f);
	// Eキーを作成
	CKeyE* keyE = new CKeyE(1144.0f, 560.0f);
	// メニューアイコンを作成
	CMenu* menu = new CMenu(10.0f, 680.0f);
	//Mキーを作成
	CKeyM* keyM = new CKeyM(40.0f, 670.0f);

	// フィールド
	CField* field = new CField();

	// 空
	CSky* sky = new CSky();
	sky->Scale(250.0f, 250.0f, 250.0f);
	sky->Position(0.0f, -1.5f, 0.0f);

	// 夜空
	CNightSky* nightSky = new CNightSky();
	nightSky->Scale(250.0f, 250.0f, 250.0f);
	nightSky->Position(0.0f, -1.5f, 0.0f);

	CPlayer* player = new CPlayer();

	// CGameCamera2のテスト
	CVector atPos = player->Position() + CVector(DEFAULT_CAMERA_POS);
	CGameCamera2* mainCamera = new CGameCamera2
	(
		atPos + CVector(DEFAULT_CAMERA_POS),
		player->Position() + CVector(0.0f, 20.0f, 0.0f)
	);
	mainCamera->SetFollowTargetTf(player);
	mainCamera->AddCollider(field->GetCollider());
	mainCamera->SetHitColliderRatio(0.98f);
	player->Position(0.0f, 50.0f, 0.0f);

	// レッドスライム
	CSlime* enemy = new CSlime();
	enemy->Position(0.0f, 0.0f, -40.0f);

	//// オレンジスライム
	//CSlime2* enemy2 = new CSlime2();
	//enemy2->Position(-40.0f, 0.0f, -40.0f);
	//enemy2->Scale(25.0f, 25.0f, 25.0f);

	//// ブルースライム
	//CSlime3* enemy3 = new CSlime3();
	//enemy3->Position(-20.0f, 0.0f, -90.0f);

	// マッシュルーム
	CMushroom* enemy4 = new CMushroom();
	enemy4->Position(150.0f, 0.0f, -150.0f);

	// 亀
	CTurtle* enemy7 = new CTurtle();
	enemy7->Position(0.0f, -0.5f, -300.0f);

	// エイ
	CRay* enemy10 = new CRay();
	enemy10->Position(150.0f, 0.0f, -450.0f);

	// 蜂
	CBee* enemy13 = new CBee();
	enemy13->Position(0.0f, 0.0f, -600.0f);

	// サボテン
	CCactus* enemy16 = new CCactus();
	enemy16->Position(150.0f, 0.0f, -750.0f);

	// チェストモンスター
	CChest* enemy19 = new CChest();
	enemy19->Position(0.0f, 0.0f, -900.0f);

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
	enemy28->Position(75.0f, 3.0f, -1350.0f);

	// ドラゴン
	CDragon* bossEnemy = new CDragon();
	bossEnemy->Position(75.0f, 1.0f, -1700.0f);

	// ゲームメニューを作成
	CGameMenu::Instance();
	CGameOverScene::Instance();
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
	CGameOverScene* gameOver = CGameOverScene::Instance();
	if (!menu->IsOpened()&& !gameOver->IsOpened())
	{
		if (CInput::PushKey('M'))
		{
			menu->Open();
		}
	}
}
