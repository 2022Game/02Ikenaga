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
#include "CBall.h"

//コンストラクタ
CGameScene::CGameScene()
	: CSceneBase(EScene::eGame)
{
}

//デストラクタ
CGameScene::~CGameScene()
{
}

//シーン読み込み
void CGameScene::Load()
{
	//ここでゲーム中に必要な
	//リソースの読み込みやクラスの生成を行う

	new CField();

	CPlayer* player = new CPlayer();

	CSword* sword = new CSword();
	sword->SetAttachMtx(player->GetFrameMtx("Armature_mixamorig_RightHand"));

	CShield* shield = new CShield();
	shield->SetAttachMtx(player->GetFrameMtx("Armature_mixamorig_LeftHand"));

	CCamera* mainCamera = new CCamera
	(
		//CVector(5.0f, -15.0f, 180.0f),
		CVector(0.0f, 50.0f, 75.0f),
		player->Position()
	);
	mainCamera->SetFollowTargetTf(player);

	CEnemy* enemy = new CEnemy();
	enemy->Position(0.0f, 0.0f, -20.0f);
	enemy->Scale(20.0f, 20.0f, 20.0f);

	//HPゲージを作成
	/*CHpGauge* hpgauge = new CHpGauge();
	hpgauge->SetPos(10.0f, 190.0f);*/

	CEnemy2* enemy2 = new CEnemy2();
	enemy2->Position(100.0f, 0.0f, 20.0f);
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

	CBall* ball = new CBall();
	ball->Position(100.0f, 10.0f, 0.0f);
	ball->Scale(4.0f, 4.0f, 4.0f);
}

//シーンの更新処理
void CGameScene::Update()
{
}
