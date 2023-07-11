#include "CStage1.h"
#include "CEnemy.h"
#include "CPolice.h"
#include "CResourceManager.h"
#include "CClear.h"
#include "CApplication.h"

bool CStage1::IsClear()
{
	return CClear::Hp() <= 0;
}

void CStage1::Clear()
{
	mpUi->Clear();
	mpUi->Hp(CPlayer::Hp());
}

bool CStage1::IsOver()
{	//HPが0以下か判定結果を戻す
	return mpPlayer->Hp() < 0;
}

void CStage1::Over()
{
	mpUi->Hp(CPlayer::Hp());
	mpUi->Over();
}

void CStage1::Start()
{
	//mpUi->Render();
}

void CStage1::Update()
{
	mpUi->Hp(CPlayer::Hp());
	//mpUi->Start();
}

//コンストラクタ
CStage1::CStage1()
	:mpPlayer(0)
	,mpUi(nullptr)
{
	mpUi = new CUi();
	//敵機のインスタンス作成
	CModel* enemyModel = GET_MODEL("Enemy1");
	AddChara(new CEnemy(enemyModel, CVector(30.0f, -1.0f, -10.0f), CVector(0.0f, 0.0f, 0.0f), CVector(0.4f, 0.4f, 0.4f)));

	CModel* policeModel = GET_MODEL("Police");
	//new CPolice(&mPolice, CVector(20.0f, 2.5f, 0.0f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));
	AddChara(new CPolice(policeModel, CVector(7.0f, 2.5f, -15.7f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f)));
}

//デストラクタ
CStage1::~CStage1()
{
	//全てのインスタンス削除
	//CApplication::CharacterManager()->AllDelete();
	//UIを生成している時
	if (mpUi != nullptr)
	{	//UIを削除し、初期化
		delete mpUi;
		mpUi = nullptr;
	}
}