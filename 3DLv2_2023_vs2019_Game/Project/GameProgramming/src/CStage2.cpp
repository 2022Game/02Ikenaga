#include "CStage2.h"
#include "CEnemy.h"
#include "CPolice.h"
#include "CResourceManager.h"

//コンストラクタ
CStage2::CStage2()
{
	//敵機のインスタンス作成
	CModel* enemyModel = GET_MODEL("Enemy1");
	AddChara(new CEnemy(enemyModel, CVector(30.0f, -1.0f, -10.0f), CVector(0.0f, 0.0f, 0.0f), CVector(0.4f, 0.4f, 0.4f)));
	AddChara(new CEnemy(enemyModel, CVector(30.0f, -1.0f, -5.0f), CVector(0.0f, 0.0f, 0.0f), CVector(0.4f, 0.4f, 0.4f)));
	AddChara(new CEnemy(enemyModel, CVector(30.0f, -1.0f, 0.0f), CVector(0.0f, 0.0f, 0.0f), CVector(0.4f, 0.4f, 0.4f)));
	AddChara(new CEnemy(enemyModel, CVector(30.0f, -1.0f, 5.0f), CVector(0.0f, 0.0f, 0.0f), CVector(0.4f, 0.4f, 0.4f)));
	AddChara(new CEnemy(enemyModel, CVector(30.0f, -1.0f, 10.0f), CVector(0.0f, 0.0f, 0.0f), CVector(0.4f, 0.4f, 0.4f)));

	CModel* policeModel = GET_MODEL("Police");
	//new CPolice(&mPolice, CVector(20.0f, 2.5f, 0.0f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));
	AddChara(new CPolice(policeModel, CVector(7.0f, 2.5f, -15.7f), CVector(0.0f, 0.0f, -15.7f), CVector(1.0f, 1.0f, 1.0f)));
	AddChara(new CPolice(policeModel, CVector(7.0f, 2.5f, -15.7f), CVector(0.0f, 0.0f, -10.7f), CVector(1.0f, 1.0f, 1.0f)));
	AddChara(new CPolice(policeModel, CVector(7.0f, 2.5f, -15.7f), CVector(0.0f, 0.0f, -5.7f), CVector(1.0f, 1.0f, 1.0f)));
	AddChara(new CPolice(policeModel, CVector(7.0f, 2.5f, -15.7f), CVector(0.0f, 0.0f, 0.7f), CVector(1.0f, 1.0f, 1.0f)));
	AddChara(new CPolice(policeModel, CVector(7.0f, 2.5f, -15.7f), CVector(0.0f, 0.0f, 5.7f), CVector(1.0f, 1.0f, 1.0f)));
}

//デストラクタ
CStage2::~CStage2()
{
}