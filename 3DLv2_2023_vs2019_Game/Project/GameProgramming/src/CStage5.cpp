#include "CStage5.h"
#include "CPlayer.h"
#include "CEnemy.h"
#include "CPolice.h"
#include "CResourceManager.h"
#include "CClear.h"
#include "CSceneBase.h"
#include "CFrame.h"
#include "CWater.h"
#include "CShark.h"
#include "CTiger.h"
#include "CMeat.h"
#include "CButton.h"
#include "CButton2.h"

//コンストラクタ
CStage5::CStage5()
{
	CModel* playerMode = GET_MODEL("Player");
	AddChara(new CPlayer(playerMode, CVector(30.0f, 0.0f, -15.5f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f)));

	//敵機のインスタンス作成
	CModel* enemyModel = GET_MODEL("Enemy1");
	//AddChara(new CEnemy(enemyModel, CVector(30.0f, -1.0f, -10.0f), CVector(0.0f, 0.0f, 0.0f), CVector(0.4f, 0.4f, 0.4f)));
	//AddChara(new CEnemy(enemyModel, CVector(30.0f, -1.0f, 10.0f), CVector(0.0f, 0.0f, 0.0f), CVector(0.4f, 0.4f, 0.4f)));
	//AddChara(new CEnemy(enemyModel, CVector(30.0f, -1.0f, -11.0f), CVector(0.0f, 0.0f, 0.0f), CVector(0.4f, 0.4f, 0.4f)));
	//AddChara(new CEnemy(enemyModel, CVector(30.0f, -1.0f, 1.0f), CVector(0.0f, 0.0f, 0.0f), CVector(0.4f, 0.4f, 0.4f)));

	//CModel* policeModel = GET_MODEL("Police");
	//AddChara(new CPolice(policeModel, CVector(7.0f, 2.5f, -15.7f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f)));

	CModel* clearModel = GET_MODEL("Clear");
	AddChara(new CClear(clearModel, CVector(30.0f, -0.9f, 50.0f), CVector(0.0f, 180.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f)));

	CModel* frameModel = GET_MODEL("Frame");
	AddChara(new CFrame(frameModel, CVector(30.0f, -0.9f, 50.0f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f)));

	CModel* waterModel = GET_MODEL("Water");
	AddChara(new CWater(waterModel, CVector(54.8f, -0.9f, 35.0f), CVector(0.0f, 90.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f)));
	AddChara(new CWater(waterModel, CVector(30.0f, -0.9f, 35.0f), CVector(0.0f, 90.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f)));
	AddChara(new CWater(waterModel, CVector(5.2f, -0.9f, 35.0f), CVector(0.0f, 90.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f)));

	CModel* sharkModel = GET_MODEL("Shark");
	AddChara(new CShark(sharkModel, CVector(10.0f, -0.5f, 35.0f), CVector(0.0f, 90.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f)));

	CModel* tigerModel = GET_MODEL("Tiger");
	AddChara(new CTiger(tigerModel, CVector(30.8f,-1.0f, 17.0f), CVector(0.0f, 180.0f, 0.0f), CVector(3.0f, 1.5f, 3.0f)));
	AddChara(new CTiger(tigerModel, CVector(29.1f, -1.0f, 17.0f), CVector(0.0f, 180.0f, 0.0f), CVector(3.0f, 1.5f, 3.0f)));

	CModel* meatModel = GET_MODEL("Meat");
	AddChara(new CMeat(meatModel, CVector(30.0f, 11.0f, -15.0f), CVector(0.0f, 180.0f, 0.0f), CVector(0.05f, 0.05f, 0.05f)));

	CModel* buttonModel = GET_MODEL("Button");
	AddChara(new CButton(buttonModel, CVector(19.0f, -1.0f, -14.0f), CVector(0.0f, 0.0f, 0.0f), CVector(3.0f, 10.0f, 3.0f)));
    AddChara(new CButton2(buttonModel, CVector(30.0f, 4.5f, -15.0f), CVector(0.0f, 0.0f, 0.0f), CVector(3.0f, 20.0f, 3.0f)));
}

//デストラクタ
CStage5::~CStage5()
{
}