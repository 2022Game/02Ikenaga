#include "CGameScene.h"
#include "CSceneManager.h"
#include "CField.h"
#include "CPlayer.h"
#include "CSword.h"
#include "CShield.h"
#include "CCamera.h"
#include "CEnemy.h"
#include "CHpGauge.h"
#include "CEnemy2.h"
#include "CPortionRed.h"
#include "CPortionGreen.h"
#include "CPortionBlue.h"
#include "CExp.h"

//コンストラクタ
CGameScene::CGameScene()
	: CSceneBase(EScene::eGame)
	,RevivalTime(0)
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

	new CField();

	CPlayer* player = new CPlayer();

	CCamera* mainCamera = new CCamera
	(
		DEFAULT_CAMERA_POS,
		player->Position()
	);
	mainCamera->SetFollowTargetTf(player);

	CEnemy* enemy = new CEnemy();
	enemy->Position(0.0f, 0.0f, -20.0f);
	enemy->Scale(25.0f, 25.0f, 25.0f);

	//HPゲージを作成
	/*CHpGauge* hpgauge = new CHpGauge();
	hpgauge->SetPos(10.0f, 190.0f);*/

	CEnemy2* enemy2 = new CEnemy2();
	enemy2->Position(-20.0f, 0.0f, 20.0f);
	enemy2->Scale(20.0f, 20.0f, 20.0f);

	CPortionRed* portionred = new CPortionRed();
	portionred->Position(90.0f, 10.0f, 0.0f);
	portionred->Scale(70.0f, 70.0f, 70.0f);

	CPortionGreen* portiongreen = new CPortionGreen();
	portiongreen->Position(80.0f, 10.0f, 0.0f);
	portiongreen->Scale(70.0f, 70.0f, 70.0f);

	CPortionBlue* portionblue = new CPortionBlue();
	portionblue->Position(70.0f, 10.0f, 0.0f);
	portionblue->Scale(70.0f, 70.0f, 70.0f);

	CExp* exp = new CExp();
	exp->Position(50.0f, 5.0f, 0.0f);
	exp->Scale(4.0f, 4.0f, 4.0f);
}

//シーンの更新処理
void CGameScene::Update()
{
	if (mpEnemy->mHp <= 0)
	{
		(RevivalTime++/100);
		if (RevivalTime > 1000)
		{
			CEnemy* enemy = new CEnemy();
			enemy->Position(0.0f, 0.0f, -20.0f);
			enemy->Scale(25.0f, 25.0f, 25.0f);
			RevivalTime = 0;
		}
	}
	CDebugPrint::Print(" 復活時間: %d\n", RevivalTime / 100);
}
