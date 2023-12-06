#include "CGameScene.h"
#include "CSceneManager.h"
#include "CField.h"
#include "CPlayer.h"
#include "CSword.h"
#include "CShield.h"
#include "CCamera.h"
#include "CSlime.h"
#include "CSlime2.h"
#include "CHpGauge.h"
#include "CEnemy2.h"
#include "CRich.h"
#include "CPortionRed.h"
#include "CPortionGreen.h"
#include "CPortionBlue.h"
#include "CCane.h"
//#include "CLineEffect.h"

//コンストラクタ
CGameScene::CGameScene()
	: CSceneBase(EScene::eGame)
	,RevivalTime(0)
	,RevivalTime2(0)
{
}

//デストラクタ
CGameScene::~CGameScene()
{
}

//シーン読み込み
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

	// その他
	//CResourceManager::Load<CImage>("Exp", "Item\\exp.png");

	new CField();

	CPlayer* player = new CPlayer();

	CCamera* mainCamera = new CCamera
	(
		DEFAULT_CAMERA_POS,
		player->Position()
	);
	mainCamera->SetFollowTargetTf(player);

	player->Position(0.0f, 5.0f, 0.0f);

	CSlime* enemy = new CSlime();
	enemy->Position(0.0f, 0.0f, -40.0f);
	enemy->Scale(25.0f, 25.0f, 25.0f);

	CSlime2* enemy2 = new CSlime2();
	enemy2->Position(-40.0f, 0.0f, -40.0f);
	enemy2->Scale(25.0f, 25.0f, 25.0f);

	//HPゲージを作成
	/*CHpGauge* hpgauge = new CHpGauge();
	hpgauge->SetPos(10.0f, 190.0f);*/

	CEnemy2* enemy5 = new CEnemy2();
	enemy5->Position(60.0f, 0.0f, 20.0f);
	enemy5->Scale(25.0f, 25.0f, 25.0f);

	CRich* enemy6 = new CRich();
	enemy6->Position(100.0f, 5.0f, 20.0f);
	enemy6->Rotate(0.0f,90.0f,-70.0f);
	enemy6->Scale(0.1f, 0.1f, 0.1f);

	CPortionRed* portionred = new CPortionRed();
	portionred->Position(90.0f, 10.0f, 0.0f);
	portionred->Scale(70.0f, 70.0f, 70.0f);

	CPortionGreen* portiongreen = new CPortionGreen();
	portiongreen->Position(80.0f, 10.0f, 0.0f);
	portiongreen->Scale(70.0f, 70.0f, 70.0f);

	CPortionBlue* portionblue = new CPortionBlue();
	portionblue->Position(70.0f, 10.0f, 0.0f);
	portionblue->Scale(70.0f, 70.0f, 70.0f);

	//CLineEffect* le = new CLineEffect(ETag::eNone);
	//le->AddPoint(CVector(0.0f, 10.0f, 10.0f), 1.0f);
	//le->AddPoint(CVector(10.0f, 10.0f, 10.0f), 1.0f);
	//le->AddPoint(CVector(10.0f, 20.0f, 50.0f), 1.0f);
	//le->AddPoint(CVector(50.0f, 10.0f, 50.0f), 1.0f);
}

//シーンの更新処理
void CGameScene::Update()
{
	if (mpSlime->mHp <= 0)
	{
		(RevivalTime++/100);
		if (RevivalTime > 1000)
		{
			CSlime* enemy = new CSlime();
			enemy->Position(0.0f, 0.0f, -20.0f);
			enemy->Scale(25.0f, 25.0f, 25.0f);
			RevivalTime = 0;
		}
	}
	if (mpSlime2->mHp <= 0)
	{
		(RevivalTime2++ / 100);
		if (RevivalTime2 > 1000)
		{
			CSlime2* enemy2 = new CSlime2();
			enemy2->Position(-40.0f, 0.0f, -20.0f);
			enemy2->Scale(25.0f, 25.0f, 25.0f);
			RevivalTime2 = 0;
		}
	}
	CDebugPrint::Print(" 復活時間: %d\n", RevivalTime / 100);
}
