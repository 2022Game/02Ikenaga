#include "COperationIns2.h"
#include "CInput.h"
#include "CTaskManager.h"
#include "CBGMManager.h"

#define MENU_ALPHA 0.75f

// コンストラ
COperationIns2::COperationIns2()
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

	mpTextOperation = new CImage
	(
		"UI/Text/operation.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpTextOperation->SetPos(105.0f, 80.0f);
	mpTextOperation->SetSize(250.0f, 80.0f);

	mpTextMove = new CImage
	(
		"UI/Text/move.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpTextMove->SetPos(600.0f, 160.0f);
	mpTextMove->SetSize(250.0f, 80.0f);

	mpTextSpecial = new CImage
	(
		"UI/Text/special.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpTextSpecial->SetPos(600.0f, 240.0f);
	mpTextSpecial->SetSize(250.0f, 80.0f);

	mpTextSP = new CImage
	(
		"UI/Text/TextSP.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpTextSP->SetPos(765.0f, 240.0f);
	mpTextSP->SetSize(250.0f, 80.0f);

	mpTextMenu = new CImage
	(
		"UI/Text/menu.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpTextMenu->SetPos(600.0f, 320.0f);
	mpTextMenu->SetSize(250.0f, 80.0f);

	mpTextJump = new CImage
	(
		"UI/Text/jump.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpTextJump->SetPos(600.0f, 400.0f);
	mpTextJump->SetSize(250.0f, 80.0f);

	mpTextAvoid = new CImage
	(
		"UI/Text/avoidance.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpTextAvoid->SetPos(600.0f, 480.0f);
	mpTextAvoid->SetSize(250.0f, 80.0f);

	mpTextMove2 = new CImage
	(
		"UI/Text/move.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpTextMove2->SetPos(130.0f, 480.0f);
	mpTextMove2->SetSize(250.0f, 80.0f);

	mpTextAttack = new CImage
	(
		"UI/Text/attack.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpTextAttack->SetPos(600.0f, 560.0f);
	mpTextAttack->SetSize(250.0f, 80.0f);

	mpTextGuard = new CImage
	(
		"UI/Text/guard.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpTextGuard->SetPos(800.0f, 560.0f);
	mpTextGuard->SetSize(250.0f, 80.0f);

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

	mpShiftKey = new CImage
	(
		"UI/Key/shift.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpShiftKey->SetPos(410.0f, 480.0f);
	mpShiftKey->SetSize(100.0f, 80.0f);

	mpMouseLeft = new CImage
	(
		"UI/Mouse/mouseLeft.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpMouseLeft->SetPos(250.0f, 560.0f);
	mpMouseLeft->SetSize(100.0f, 80.0f);

	mpMouseRight = new CImage
	(
		"UI/Mouse/mouseRight.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpMouseRight->SetPos(350.0f, 560.0f);
	mpMouseRight->SetSize(100.0f, 80.0f);

	mpPlus = new CImage
	(
		"UI/plus.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpPlus->SetPos(305.0f, 480.0f);
	mpPlus->SetSize(90.0f, 80.0f);

	mpBack = new CImage
	(
		"UI/back.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpBack->SetPos(140.0f, 600.0f);
	mpBack->SetSize(30.0f, 30.0f);

	mpShiftKey2 = new CImage
	(
		"UI/Key/shift.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpShiftKey2->SetPos(185.0f, 590.0f);
	mpShiftKey2->SetSize(55.0f, 55.0f);

	SetEnable(false);
	SetShow(false);
}

// デストラクタ
COperationIns2::~COperationIns2()
{
	SAFE_DELETE(mpBackground);
	SAFE_DELETE(mpTextOperation);
	SAFE_DELETE(mpTextMove);
	SAFE_DELETE(mpTextSpecial);
	SAFE_DELETE(mpTextSP);
	SAFE_DELETE(mpTextMenu);
	SAFE_DELETE(mpTextJump);
	SAFE_DELETE(mpTextAvoid);
	SAFE_DELETE(mpTextMove2);
	SAFE_DELETE(mpTextAttack);
	SAFE_DELETE(mpTextGuard);

	SAFE_DELETE(mpWkey);
	SAFE_DELETE(mpAkey);
	SAFE_DELETE(mpSkey);
	SAFE_DELETE(mpDkey);
	SAFE_DELETE(mpQkey);
	SAFE_DELETE(mpEkey);
	SAFE_DELETE(mpMkey);
	SAFE_DELETE(mpSpaceKey);
	SAFE_DELETE(mpShiftKey);
	SAFE_DELETE(mpShiftKey2);

	SAFE_DELETE(mpMouseLeft);
	SAFE_DELETE(mpMouseRight);

	SAFE_DELETE(mpPlus);
	SAFE_DELETE(mpBack);
}

// オープン
void COperationIns2::Open()
{
	SetEnable(true);
	SetShow(true);
	mIsOpened = true;
	CInput::ShowCursor(false);
}

// クローズ
void COperationIns2::Close()
{
	SetEnable(false);
	SetShow(false);
	CInput::ShowCursor(true);
}

// オープンするかどうか
bool COperationIns2::IsOpened() const
{
	return mIsOpened;
}

// 決める
void COperationIns2::Decide(int select)
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
void COperationIns2::Update()
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
void COperationIns2::Render()
{
	mpBackground->Render();
	mpTextOperation->Render();
	mpTextMove->Render();
	mpTextSpecial->Render();
	mpTextSP->Render();
	mpTextMenu->Render();
	mpTextJump->Render();
	mpTextAvoid->Render();
	mpTextMove2->Render();
	mpTextAttack->Render();
	mpTextGuard->Render();

	mpWkey->Render();
	mpAkey->Render();
	mpSkey->Render();
	mpDkey->Render();
	mpQkey->Render();
	mpEkey->Render();
	mpMkey->Render();
	mpSpaceKey->Render();
	mpShiftKey->Render();
	mpShiftKey2->Render();

	mpMouseLeft->Render();
	mpMouseRight->Render();

	mpPlus->Render();
	mpBack->Render();
}