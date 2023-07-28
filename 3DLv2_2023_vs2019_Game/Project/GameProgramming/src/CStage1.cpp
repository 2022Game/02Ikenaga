#include "CStage1.h"
#include "CPlayer.h"
#include "CEnemy.h"
#include "CPolice.h"
#include "CResourceManager.h"
#include "CClear.h"
#include "CApplication.h"
#include "CSceneBase.h"
#include "CFrame.h"
#include "CSceneManager.h"
#include "CSp.h"
#include "CNaihu.h"

//コンストラクタ
CStage1::CStage1()
{
	CModel* playerMode = GET_MODEL("Player");
	AddChara(new CPlayer(playerMode, CVector(30.0f, 0.0f, -15.5f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f)));

	//敵機のインスタンス作成
	CModel* enemyModel = GET_MODEL("Enemy1");
	AddChara(new CEnemy(enemyModel, CVector(30.0f, -1.0f, -10.0f), CVector(0.0f, 0.0f, 0.0f), CVector(0.4f, 0.4f, 0.4f)));

	//CModel* policeModel = GET_MODEL("Police");
	//AddChara(new CPolice(policeModel, CVector(7.0f, 2.5f, -15.7f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f)));

	CModel* sp = GET_MODEL("SP");
	AddChara(new CSp(sp, CVector(30.0f, -1.0f, -10.0f), CVector(0.0f, 180.0f, 0.0f), CVector(0.048f, 0.048f, 0.048f)));

	CModel* clearModel = GET_MODEL("Clear");
	AddChara(new CClear(clearModel, CVector(30.0f, -0.9f, 30.0f), CVector(0.0f, 180.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f)));

	CModel* frameModel = GET_MODEL("Frame");
	AddChara(new CFrame(frameModel, CVector(30.0f, -0.9f, 30.0f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f)));
}

//デストラクタ
CStage1::~CStage1()
{
}

void CStage1::Update()
{
}