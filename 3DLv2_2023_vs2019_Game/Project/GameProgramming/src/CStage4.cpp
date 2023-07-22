#include "CStage4.h"
#include "CPlayer.h"
#include "CEnemy.h"
#include "CPolice.h"
#include "CResourceManager.h"
#include "CClear.h"
#include "CApplication.h"
#include "CSceneBase.h"
#include "CFrame.h"
#include "CWater.h"
#include "CShark.h"

//コンストラクタ
CStage4::CStage4()
{
	CModel* playerMode = GET_MODEL("Player");
	AddChara(new CPlayer(playerMode, CVector(30.0f, 0.0f, -15.5f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f)));

	//敵機のインスタンス作成
	CModel* enemyModel = GET_MODEL("Enemy1");
	AddChara(new CEnemy(enemyModel, CVector(30.0f, -1.0f, -10.0f), CVector(0.0f, 0.0f, 0.0f), CVector(0.4f, 0.4f, 0.4f)));
	//AddChara(new CEnemy(enemyModel, CVector(30.0f, -1.0f, 10.0f), CVector(0.0f, 0.0f, 0.0f), CVector(0.4f, 0.4f, 0.4f)));
	//AddChara(new CEnemy(enemyModel, CVector(30.0f, -1.0f, -11.0f), CVector(0.0f, 0.0f, 0.0f), CVector(0.4f, 0.4f, 0.4f)));
	//AddChara(new CEnemy(enemyModel, CVector(30.0f, -1.0f, 1.0f), CVector(0.0f, 0.0f, 0.0f), CVector(0.4f, 0.4f, 0.4f)));

	CModel* policeModel = GET_MODEL("Police");
	AddChara(new CPolice(policeModel, CVector(7.0f, 2.5f, -15.7f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f)));

	CModel* clearModel = GET_MODEL("Clear");
	AddChara(new CClear(clearModel, CVector(30.0f, -0.9f, 50.0f), CVector(0.0f, 180.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f)));

	CModel* frameModel = GET_MODEL("Frame");
	AddChara(new CFrame(frameModel, CVector(30.0f, -0.9f, 50.0f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f)));

	CModel* waterModel = GET_MODEL("Water");
	AddChara(new CWater(waterModel, CVector(54.8f, -0.9f, 35.0f), CVector(0.0f, 90.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f)));
	AddChara(new CWater(waterModel, CVector(30.0f, -0.9f, 35.0f), CVector(0.0f, 90.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f)));
	AddChara(new CWater(waterModel, CVector(5.2f, -0.9f, 35.0f), CVector(0.0f, 90.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f)));

	CModel* sharkModel = GET_MODEL("Shark");
	AddChara(new CShark(sharkModel, CVector(10.0f,-0.5f, 35.0f), CVector(0.0f, 90.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f)));
}

//デストラクタ
CStage4::~CStage4()
{
}