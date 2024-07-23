#include "CGameMenu.h"
#include "CInput.h"
#include "CTaskManager.h"
#include "CBGMManager.h"
#include "COperationIns.h"
#include "CPlayerStatusUI.h"

CGameMenu* CGameMenu::spInstance = nullptr;

#define MENU_ALPHA 0.75f

// ゲームメニューのインスタンスを取得
CGameMenu* CGameMenu::Instance()
{
	if (spInstance == nullptr)
	{
		spInstance = new CGameMenu();
	}
	return spInstance;
}

// ゲームメニューのインスタンスを削除
void CGameMenu::ClearInstance()
{
	if (spInstance != nullptr)
	{
		spInstance->Kill();
		spInstance = nullptr;
	}
}

// コンストラ
CGameMenu::CGameMenu()
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eMenu)
	, mSelectIndex(0)
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

	int menuItemCount = 3;
	float spaceY = (float)WINDOW_HEIGHT / (menuItemCount + 1);
	for (int i = 0; i < menuItemCount; i++)
	{
		CImage* item = new CImage
		(
			"UI/menu_item.png",
			ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
			false, false
		);
		item->SetCenter(item->GetSize() * 0.5f);
		float posX = (1280.0f - 1024.0f + item->GetSize().X()) * 0.5f + 100.0f;
		item->SetPos(posX, spaceY * (i + 1));
		item->SetColor(1.0f, 1.0f, 1.0f, MENU_ALPHA);
		mMenuItems.push_back(item);
	}

	mpSelectFrame = new CImage
	(
		"UI/menu_item_select.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpSelectFrame->SetCenter(mpSelectFrame->GetSize() * 0.5f);
	mpSelectFrame->SetColor(0.0f, 0.7f, 1.0f, MENU_ALPHA);

	mpTextOperation = new CImage
	(
		"UI/Text/operation.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpTextOperation->SetCenter(mpSelectFrame->GetSize() * 0.5f);
	mpTextOperation->SetPos(485.0f, 380.0f);
	mpTextOperation->SetSize(350.0f, 90.0f);

	mpStatus = new CImage
	(
		"UI/Text/status.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpStatus->SetCenter(mpSelectFrame->GetSize() * 0.5f);
	mpStatus->SetPos(480.0f, 197.0f);
	mpStatus->SetSize(350.0f, 90.0f);

	mpTextReturn = new CImage
	(
		"UI/Text/TextReturn.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpTextReturn->SetCenter(mpSelectFrame->GetSize() * 0.5f);
	mpTextReturn->SetPos(485.0f, 557.0f);
	mpTextReturn->SetSize(350.0f, 90.0f);

	mpUpUI = new CImage
	(
		"UI/up.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpUpUI->SetPos(1080.0f, 560.0f);
	mpUpUI->SetSize(60.0f, 60.0f);

	mpDownUI = new CImage
	(
		"UI/down.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpDownUI->SetPos(1080.0f, 590.0f);
	mpDownUI->SetSize(60.0f, 60.0f);

	mpOperationIns = new COperationIns();
	mpPlayerStatusUI = new CPlayerStatusUI();

	SetEnable(false);
	SetShow(false);
}

// デストラクタ
CGameMenu::~CGameMenu()
{
	SAFE_DELETE(mpBackground);
	SAFE_DELETE(mpSelectFrame);
	SAFE_DELETE(mpTextOperation);
	SAFE_DELETE(mpStatus);
	SAFE_DELETE(mpTextReturn);
	SAFE_DELETE(mpUpUI);
	SAFE_DELETE(mpDownUI);
}

// オープン
void CGameMenu::Open()
{
	SetEnable(true);
	SetShow(true);
	mIsOpened = true;
	mSelectIndex = 0;
	CBGMManager::Instance()->Play(EBGMType::eMenu, false);
	CTaskManager::Instance()->Pause(PAUSE_MENU_OPEN);
}

// クローズ
void CGameMenu::Close()
{
	SetEnable(false);
	SetShow(false);
	mIsOpened = false;
	CBGMManager::Instance()->Play(EBGMType::eGame, false);
	CTaskManager::Instance()->UnPause(PAUSE_MENU_OPEN);
}

// オープンするかどうか
bool CGameMenu::IsOpened() const
{
	return mIsOpened;
}

// 決める
void CGameMenu::Decide(int select)
{
	switch (select)
	{
		case 0:
		case 1:
			break;
		case 2:
			Close();
			break;
	}
}

// 更新
void CGameMenu::Update()
{
	int itemCount = mMenuItems.size();
	if (CInput::PushKey(VK_UP))
	{
		mSelectIndex = (mSelectIndex + itemCount - 1) % itemCount;
	}
	else if (CInput::PushKey(VK_DOWN))
	{
		mSelectIndex = (mSelectIndex + 1) % itemCount;
	}
	else if (CInput::PushKey(VK_RETURN))
	{
		Decide(mSelectIndex);
	}

	switch (mSelectIndex)
	{
	case 0:
		if (CInput::PushKey(VK_SPACE))
		{
			mpPlayerStatusUI->Open();
			mIsOpened = true;
			SetEnable(false);
			SetShow(false);
		}
		break;
	case 1:
		if (CInput::PushKey(VK_SPACE))
		{
			mpOperationIns->Open();
			mIsOpened = true;
			SetEnable(false);
			SetShow(false);
		}
		break;
	}

	mpBackground->Update();
	for (CImage* item : mMenuItems)
	{
		item->Update();
	}
	mpSelectFrame->Update();
}

// 描画
void CGameMenu::Render()
{
	mpBackground->Render();
	for (int i = 0; i < mMenuItems.size(); i++)
	{
		CImage* item = mMenuItems[i];
		item->Render();

		if (i == mSelectIndex)
		{
			mpSelectFrame->SetPos(item->GetPos());
			mpSelectFrame->Render();
		}
	}
	mpTextOperation->Render();
	mpStatus->Render();
	mpTextReturn->Render();
	mpUpUI->Render();
	mpDownUI->Render();
}
