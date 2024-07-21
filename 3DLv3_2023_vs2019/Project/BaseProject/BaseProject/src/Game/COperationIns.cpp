#include "COperationIns.h"
#include "CInput.h"
#include "CTaskManager.h"
#include "CBGMManager.h"
#include "CGameMenu.h"

#define MENU_ALPHA 0.75f

// コンストラ
COperationIns::COperationIns()
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eMenu)
	, mIsOpened(false)
{
	mpBackground = new CImage
	(
		"UI/menu_back.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpBackground->SetCenter(mpBackground->GetSize() * 0.5f);
	mpBackground->SetPos(CVector2(WINDOW_WIDTH, WINDOW_HEIGHT) * 0.5f);
	mpBackground->SetColor(0.0f, 0.0f, 0.0f, MENU_ALPHA);

	mpText = new CImage
	(
		"UI/Text/Text.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpText->SetPos(105.0f, 80.0f);
	mpText->SetSize(250.0f, 80.0f);

	mpText2 = new CImage
	(
		"UI/Text/Text2.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpText2->SetPos(600.0f, 160.0f);
	mpText2->SetSize(250.0f, 80.0f);

	mpText3 = new CImage
	(
		"UI/Text/Text3.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpText3->SetPos(600.0f, 240.0f);
	mpText3->SetSize(250.0f, 80.0f);

	mpTextSP = new CImage
	(
		"UI/Text/TextSP.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpTextSP->SetPos(765.0f, 240.0f);
	mpTextSP->SetSize(250.0f, 80.0f);

	mpText4 = new CImage
	(
		"UI/Text/Text4.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpText4->SetPos(600.0f, 320.0f);
	mpText4->SetSize(250.0f, 80.0f);

	mpText5 = new CImage
	(
		"UI/Text/Text5.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpText5->SetPos(600.0f, 400.0f);
	mpText5->SetSize(250.0f, 80.0f);

	mpText6 = new CImage
	(
		"UI/Text/Text6.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpText6->SetPos(600.0f, 510.0f);
	mpText6->SetSize(250.0f, 80.0f);

	mpText7 = new CImage
	(
		"UI/Text/Text7.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpText7->SetPos(800.0f, 510.0f);
	mpText7->SetSize(250.0f, 80.0f);

	mpWkey = new CImage
	(
		"UI/Key/w.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpWkey->SetPos(200.0f, 160.0f);
	mpWkey->SetSize(100.0f, 80.0f);

	mpAkey = new CImage
	(
		"UI/Key/a.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpAkey->SetPos(280.0f, 160.0f);
	mpAkey->SetSize(100.0f, 80.0f);

	mpSkey = new CImage
	(
		"UI/Key/s.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpSkey->SetPos(360.0f, 160.0f);
	mpSkey->SetSize(100.0f, 80.0f);

	mpDkey = new CImage
	(
		"UI/Key/d.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpDkey->SetPos(440.0f, 160.0f);
	mpDkey->SetSize(100.0f, 80.0f);

	mpQkey = new CImage
	(
		"UI/Key/q.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpQkey->SetPos(200.0f, 240.0f);
	mpQkey->SetSize(100.0f, 80.0f);

	mpEkey = new CImage
	(
		"UI/Key/e.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpEkey->SetPos(280.0f, 240.0f);
	mpEkey->SetSize(100.0f, 80.0f);

	mpMkey = new CImage
	(
		"UI/Key/m.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpMkey->SetPos(200.0f, 320.0f);
	mpMkey->SetSize(100.0f, 80.0f);

	mpSpaceKey = new CImage
	(
		"UI/Key/space.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpSpaceKey->SetPos(215.0f, 400.0f);
	mpSpaceKey->SetSize(100.0f, 80.0f);

	mpMouseLeft = new CImage
	(
		"UI/Mouse/mouseLeft.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpMouseLeft->SetPos(200.0f, 500.0f);
	mpMouseLeft->SetSize(100.0f, 100.0f);

	mpMouseMiddle = new CImage
	(
		"UI/Mouse/mouseMiddle.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpMouseMiddle->SetPos(370.0f, 240.0f);
	mpMouseMiddle->SetSize(100.0f, 80.0f);

	mpMouseRight = new CImage
	(
		"UI/Mouse/mouseRight.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpMouseRight->SetPos(300.0f, 500.0f);
	mpMouseRight->SetSize(100.0f, 100.0f);

	SetEnable(false);
	SetShow(false);
}

// デストラクタ
COperationIns::~COperationIns()
{
}

// オープン
void COperationIns::Open()
{
	SetEnable(true);
	SetShow(true);
	mIsOpened = true;
	CBGMManager::Instance()->Play(EBGMType::eMenu, false);
	CTaskManager::Instance()->Pause(PAUSE_MENU_OPEN);
}

// クローズ
void COperationIns::Close()
{
	SetEnable(false);
	SetShow(false);
	CBGMManager::Instance()->Play(EBGMType::eGame, false);
	CTaskManager::Instance()->UnPause(PAUSE_MENU_OPEN);
}

// オープンするかどうか
bool COperationIns::IsOpened() const
{
	return mIsOpened;
}

// 決める
void COperationIns::Decide(int select)
{
	switch (select)
	{
	case 0:
		Close();
		break;
	case 1:
		break;
	}
}

// 更新
void COperationIns::Update()
{
	mpBackground->Update();

	switch (mIsOpened)
	{
	case 0:
		Close();
		break;
	case 1:
		if (CInput::PushKey(VK_SHIFT))
		{
			mIsOpened = false;
		}
		break;
	}
}

// 描画
void COperationIns::Render()
{
	mpBackground->Render();
	mpText->Render();
	mpText2->Render();
	mpText3->Render();
	mpTextSP->Render();
	mpText4->Render();
	mpText5->Render();
	mpText6->Render();
	mpText7->Render();

	mpWkey->Render();
	mpAkey->Render();
	mpSkey->Render();
	mpDkey->Render();
	mpQkey->Render();
	mpEkey->Render();
	mpMkey->Render();
	mpSpaceKey->Render();
	mpMouseLeft->Render();
	mpMouseMiddle->Render();
	mpMouseRight->Render();
}