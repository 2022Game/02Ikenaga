#include "CGameClearScene.h"
#include "CInput.h"
#include "CTaskManager.h"
#include "CBGMManager.h"
#include "CPlayer.h"
#include "CSceneManager.h"

CGameClearScene* CGameClearScene::spInstance = nullptr;

#define SHADOW_COUNT 8     // 影のカウント
#define SHADOW_WIDTH 2.0f  // 影の幅
#define MENU_ALPHA 0.75f
#define GAMECLEAR_POS CVector2(410.0f + SHADOW_WIDTH, 150.0f)
#define TITLE_POS CVector2(540.0f + SHADOW_WIDTH, 365.0f)

// インスタンスを取得
CGameClearScene* CGameClearScene::Instance()
{
	if (spInstance == nullptr)
	{
		spInstance = new CGameClearScene();
	}
	return spInstance;
}

// インスタンスを削除
void CGameClearScene::ClearInstance()
{
	if (spInstance != nullptr)
	{
		spInstance->Kill();
		spInstance = nullptr;
	}
}

// コンストラ
CGameClearScene::CGameClearScene()
	: CTask(ETaskPriority::eUI, 1, ETaskPauseType::eGameClear)
	, mSelectIndex(0)
	, mIsOpened(false)
{
	mpBackground = new CImage
	(
		"UI/menu_back.png",
		ETaskPriority::eUI, 1, ETaskPauseType::eGameClear,
		false, false
	);
	mpBackground->SetCenter(mpBackground->GetSize() * 0.5f);
	mpBackground->SetPos(CVector2(WINDOW_WIDTH, WINDOW_HEIGHT) * 0.5f);
	mpBackground->SetColor(0.0f, 0.0f, 0.0f, MENU_ALPHA);

	mpBackground2 = new CImage
	(
		"UI/menu_back.png",
		ETaskPriority::eUI, 1, ETaskPauseType::eGameClear,
		false, false
	);
	mpBackground2->SetSize(1380.0f, 820.0f);
	mpBackground2->SetCenter(mpBackground2->GetSize() * 0.5f);
	mpBackground2->SetPos(CVector2(WINDOW_WIDTH, WINDOW_HEIGHT) * 0.5f);
	mpBackground2->SetColor(0.0f, 1.0f, 0.0f, MENU_ALPHA);

	CImage* item = new CImage
	(
		"UI/menu_item.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eGameClear,
		false, false
	);
	item->SetCenter(item->GetSize() * 0.5f);
	float posX = (1280.0f - 1024.0f + item->GetSize().X()) * 0.5f + 300.0f;
	item->SetPos(posX, 400.0f);
	item->SetColor(1.0f, 1.0f, 1.0f, MENU_ALPHA);
	mMenuItems.push_back(item);

	mpSelectFrame = new CImage
	(
		"UI/menu_item_select.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eGameClear,
		false, false
	);
	mpSelectFrame->SetCenter(mpSelectFrame->GetSize() * 0.5f);
	mpSelectFrame->SetColor(0.0f, 0.7f, 1.0f, MENU_ALPHA);

	CVector2 size = CVector2(WINDOW_WIDTH * 0.5, WINDOW_HEIGHT * 0.5f);
	ETextAlignH textAlignH = ETextAlignH::eLeft;

	mpGameClear = new CText
	(
		nullptr, 24,
		GAMECLEAR_POS,
		size,
		CColor(1.0f, 1.0f, 1.0f),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGameClear,
		false, false
	);
	mpGameClear->SetTextAlignH(textAlignH);
	mpGameClear->SetText("ゲームクリア");
	mpGameClear->SetFontSize(80);

	mpTitle = new CText
	(
		nullptr, 24,
		TITLE_POS,
		size,
		CColor(1.0f, 1.0f, 1.0f),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGameClear,
		false, false
	);
	mpTitle->SetTextAlignH(textAlignH);
	mpTitle->SetText("タイトル");
	mpTitle->SetFontSize(50);

	mpUpUI = new CImage
	(
		"UI/up.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eGameClear,
		false, false
	);
	mpUpUI->SetPos(130.0f, 560.0f);
	mpUpUI->SetSize(60.0f, 60.0f);

	mpDownUI = new CImage
	(
		"UI/down.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eGameClear,
		false, false
	);
	mpDownUI->SetPos(130.0f, 590.0f);
	mpDownUI->SetSize(60.0f, 60.0f);

	mpEnterKey = new CImage
	(
		"UI/Key/enter.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eGameClear,
		false, false
	);
	mpEnterKey->SetPos(1075.0f, 580.0f);
	mpEnterKey->SetSize(60.0f, 60.0f);

	SetEnable(false);
	SetShow(false);
}

// デストラクタ
CGameClearScene::~CGameClearScene()
{
	SAFE_DELETE(mpBackground);
	SAFE_DELETE(mpBackground2);
	SAFE_DELETE(mpSelectFrame);
	SAFE_DELETE(mpGameClear);
	SAFE_DELETE(mpTitle);
	SAFE_DELETE(mpUpUI);
	SAFE_DELETE(mpDownUI);
	SAFE_DELETE(mpEnterKey);
}

// オープン
void CGameClearScene::Open()
{
	SetEnable(true);
	SetShow(true);
	mIsOpened = true;
	mSelectIndex = 0;

	CBGMManager::Instance()->Play(EBGMType::eMenu, false);
	CTaskManager::Instance()->Pause(PAUSE_MENU_OPEN);
}

// クローズ
void CGameClearScene::Close()
{
	SetEnable(false);
	SetShow(false);
	mIsOpened = false;

	CTaskManager::Instance()->UnPause(PAUSE_MENU_OPEN);
}

// オープンするかどうか
bool CGameClearScene::IsOpened() const
{
	return mIsOpened;
}

// 決める
void CGameClearScene::Decide(int select)
{
	switch (select)
	{
	case 0:
		mIsOpened = false;

		CTaskManager::Instance()->UnPause(PAUSE_MENU_OPEN);
		CSceneManager::Instance()->LoadScene(EScene::eTitle);
		break;
	case 1:
		break;
	}
}

// 更新
void CGameClearScene::Update()
{
	if (!mIsOpened)return;

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

	mpBackground2->Update();
	mpBackground->Update();
	for (CImage* item : mMenuItems)
	{
		item->Update();
	}
	mpSelectFrame->Update();
}

// 描画
void CGameClearScene::Render()
{
	mpBackground2->Render();
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

	mpGameClear->SetColor(1.0f, 1.0f, 1.0f);
	mpTitle->SetColor(1.0f, 1.0f, 1.0f);

	for (int i = 0; i < SHADOW_COUNT; i++)
	{
		CVector2 v = CVector2(1.0f, 0.0f);
		float alpha = (float)i / SHADOW_COUNT;
		CQuaternion rot = CQuaternion(0.0f, 0.0f, 360.0f * alpha);
		mpGameClear->SetPos(GAMECLEAR_POS + (CVector2)(rot * v) * SHADOW_WIDTH);
		mpGameClear->Render();

		mpTitle->SetPos(TITLE_POS + (CVector2)(rot * v) * SHADOW_WIDTH);
		mpTitle->Render();
	}
	mpGameClear->SetColor(0.0f, 1.0f, 0.0f);
	mpGameClear->SetPos(GAMECLEAR_POS);
	mpGameClear->Render();

	mpTitle->SetColor(0.0f, 0.0f, 0.0f);
	mpTitle->SetPos(TITLE_POS);
	mpTitle->Render();

	mpUpUI->Render();
	mpDownUI->Render();
	mpEnterKey->Render();
}