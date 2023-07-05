#include "CStage1.h"
#include "CEnemy.h"
#include "CPolice.h"
#include "CResourceManager.h"
#include "CClear.h"
#include "CApplication.h"

bool CStage1::IsClear()
{
	return  CClear::Hp() <= 0;
}

void CStage1::Clear()
{
	mpUi->Clear();
}

bool CStage1::IsOver()
{	//HP��0�ȉ������茋�ʂ�߂�
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
	//mpUi->Start();
}

void CStage1::Update()
{
	mpUi->Hp(CPlayer::Hp());
}

//�R���X�g���N�^
CStage1::CStage1()
	:mpPlayer(0)
	,mpUi(nullptr)
{
	mpUi = new CUi();
	//�G�@�̃C���X�^���X�쐬
	CModel* enemyModel = GET_MODEL("Enemy1");
	AddChara(new CEnemy(enemyModel, CVector(30.0f, -1.0f, -10.0f), CVector(0.0f, 0.0f, 0.0f), CVector(0.4f, 0.4f, 0.4f)));

	CModel* policeModel = GET_MODEL("Police");
	//new CPolice(&mPolice, CVector(20.0f, 2.5f, 0.0f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));
	AddChara(new CPolice(policeModel, CVector(7.0f, 2.5f, -15.7f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f)));
}

//�f�X�g���N�^
CStage1::~CStage1()
{
	//�S�ẴC���X�^���X�폜
	//CApplication::CharacterManager()->AllDelete();
	//UI�𐶐����Ă��鎞
	if (mpUi != nullptr)
	{	//UI���폜���A������
		delete mpUi;
		mpUi = nullptr;
	}
}