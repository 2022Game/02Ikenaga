#include "CGameOverScene.h"
#include "CInput.h"
#include "CTaskManager.h"
#include "CBGMManager.h"
#include "CPlayer.h"
#include "CSceneManager.h"

CGameOverScene* CGameOverScene::spInstance = nullptr;

#define SHADOW_COUNT 8     // 影のカウント
#define SHADOW_WIDTH 2.0f  // 影の幅
#define MENU_ALPHA 0.75f
#define GAMEOVER_POS CVector2(350.0f + SHADOW_WIDTH, 150.0f)
#define REVIVAL_POS CVector2(590.0f + SHADOW_WIDTH, 365.0f)
#define END_POS CVector2(520.0f + SHADOW_WIDTH, 475.0f)

// インスタンスを取得
CGameOverScene* CGameOverScene::Instance()
{
	if (spInstance == nullptr)
	{
		spInstance = new CGameOverScene();
	}
	return spInstance;
}

// インスタンスを削除
void CGameOverScene::ClearInstance()
{
	if (spInstance != nullptr)
	{
		spInstance->Kill();
		spInstance = nullptr;
	}
}

// コンストラ
CGameOverScene::CGameOverScene()
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eGameOver)
	, mSelectIndex(0)
	, mIsOpened(false)
{
	mpBackground = new CImage
	(
		"UI/menu_back.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eGameOver,
		false, false
	);
	//mpBackground->SetSize(1380.0f, 820.0f);
	mpBackground->SetCenter(mpBackground->GetSize() * 0.5f);
	mpBackground->SetPos(CVector2(WINDOW_WIDTH, WINDOW_HEIGHT) * 0.5f);
	mpBackground->SetColor(0.0f, 0.0f, 0.0f, MENU_ALPHA);

	mpBackground2 = new CImage
	(
		"UI/menu_back.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eGameOver,
		false, false
	);
	mpBackground2->SetSize(1380.0f, 820.0f);
	mpBackground2->SetCenter(mpBackground2->GetSize() * 0.5f);
	mpBackground2->SetPos(CVector2(WINDOW_WIDTH, WINDOW_HEIGHT) * 0.5f);
	mpBackground2->SetColor(1.0f, 0.0f, 0.0f, MENU_ALPHA);

	CImage* item = new CImage
	(
		"UI/menu_item.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eGameOver,
		false, false
	);
	item->SetCenter(item->GetSize() * 0.5f);
	float posX = (1280.0f - 1024.0f + item->GetSize().X()) * 0.5f + 300.0f;
	item->SetPos(posX, 400.0f);
	item->SetColor(1.0f, 1.0f, 1.0f, MENU_ALPHA);
	mMenuItems.push_back(item);

	CImage* item2 = new CImage
	(
		"UI/menu_item.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eGameOver,
		false, false
	);
	item2->SetCenter(item2->GetSize() * 0.5f);
	item2->SetPos(posX, 510.0f);
	item2->SetColor(1.0f, 1.0f, 1.0f, MENU_ALPHA);
	mMenuItems.push_back(item2);

	mpSelectFrame = new CImage
	(
		"UI/menu_item_select.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eGameOver,
		false, false
	);
	mpSelectFrame->SetCenter(mpSelectFrame->GetSize() * 0.5f);
	mpSelectFrame->SetColor(0.0f, 0.7f, 1.0f, MENU_ALPHA);

	CVector2 size = CVector2(WINDOW_WIDTH * 0.5, WINDOW_HEIGHT * 0.5f);
	ETextAlignH textAlignH = ETextAlignH::eLeft;

	mpGameOver = new CText
	(
		nullptr, 24,
		GAMEOVER_POS,
		size,
		CColor(1.0f, 1.0f, 1.0f),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpGameOver->SetTextAlignH(textAlignH);
	mpGameOver->SetText("ゲームオーバー");
	mpGameOver->SetFontSize(80);

	mpRevival = new CText
	(
		nullptr, 24,
		REVIVAL_POS,
		size,
		CColor(1.0f, 1.0f, 1.0f),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpRevival->SetTextAlignH(textAlignH);
	mpRevival->SetText("復活");
	mpRevival->SetFontSize(50);

	mpEnd = new CText
	(
		nullptr, 24,
		END_POS,
		size,
		CColor(1.0f, 1.0f, 1.0f),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpEnd->SetTextAlignH(textAlignH);
	mpEnd->SetText("ゲーム終了");
	mpEnd->SetFontSize(50);

	mpUpUI = new CImage
	(
		"UI/up.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eGameOver,
		false, false
	);
	mpUpUI->SetPos(130.0f, 560.0f);
	mpUpUI->SetSize(60.0f, 60.0f);

	mpDownUI = new CImage
	(
		"UI/down.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eGameOver,
		false, false
	);
	mpDownUI->SetPos(130.0f, 590.0f);
	mpDownUI->SetSize(60.0f, 60.0f);

	mpEnterKey = new CImage
	(
		"UI/Key/enter.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eGameOver,
		false, false
	);
	mpEnterKey->SetPos(1075.0f, 580.0f);
	mpEnterKey->SetSize(60.0f, 60.0f);

	SetEnable(false);
	SetShow(false);
}

// デストラクタ
CGameOverScene::~CGameOverScene()
{
	SAFE_DELETE(mpBackground);
	SAFE_DELETE(mpBackground2);
	SAFE_DELETE(mpSelectFrame);
	SAFE_DELETE(mpGameOver);
	SAFE_DELETE(mpRevival);
	SAFE_DELETE(mpEnd);
	SAFE_DELETE(mpUpUI);
	SAFE_DELETE(mpDownUI);
	SAFE_DELETE(mpEnterKey);
}

// オープン
void CGameOverScene::Open()
{
	SetEnable(true);
	SetShow(true);
	mIsOpened = true;
	mSelectIndex = 0;
	CBGMManager::Instance()->Play(EBGMType::eMenu, false);
	CTaskManager::Instance()->Pause(PAUSE_MENU_OPEN);
}

// クローズ
void CGameOverScene::Close()
{
	SetEnable(false);
	SetShow(false);
	mIsOpened = false;

	CBGMManager::Instance()->Play(EBGMType::eGame, false);
	CTaskManager::Instance()->UnPause(PAUSE_MENU_OPEN);

	CPlayer* player = CPlayer::Instance();
	player->UpdateRevival();
}

// オープンするかどうか
bool CGameOverScene::IsOpened() const
{
	return mIsOpened;
}

// 決める
void CGameOverScene::Decide(int select)
{
	switch (select)
	{
	case 0:
		Close();
		break;
	case 1:
		System::ExitGame();
		break;
	}
}

// 更新
void CGameOverScene::Update()
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

	mpBackground2->Update();
	mpBackground->Update();
	for (CImage* item : mMenuItems)
	{
		item->Update();
	}
	mpSelectFrame->Update();
}

// 描画
void CGameOverScene::Render()
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

	mpGameOver->SetColor(1.0f, 1.0f, 1.0f);
	mpRevival->SetColor(1.0f, 1.0f, 1.0f);
	mpEnd->SetColor(1.0f, 1.0f, 1.0f);

	for (int i = 0; i < SHADOW_COUNT; i++)
	{
		CVector2 v = CVector2(1.0f, 0.0f);
		float alpha = (float)i / SHADOW_COUNT;
		CQuaternion rot = CQuaternion(0.0f, 0.0f, 360.0f * alpha);
		mpGameOver->SetPos(GAMEOVER_POS + (CVector2)(rot * v) * SHADOW_WIDTH);
		mpGameOver->Render();

		mpRevival->SetPos(REVIVAL_POS + (CVector2)(rot * v) * SHADOW_WIDTH);
		mpRevival->Render();

		mpEnd->SetPos(END_POS + (CVector2)(rot * v) * SHADOW_WIDTH);
		mpEnd->Render();
	}
	mpGameOver->SetColor(1.0f, 0.0f, 0.0f);
	mpGameOver->SetPos(GAMEOVER_POS);
	mpGameOver->Render();

	mpRevival->SetColor(0.0f, 0.0f, 0.0f);
	mpRevival->SetPos(REVIVAL_POS);
	mpRevival->Render();

	mpEnd->SetColor(0.0f, 0.0f, 0.0f);
	mpEnd->SetPos(END_POS);
	mpEnd->Render();

	mpUpUI->Render();
	mpDownUI->Render();
	mpEnterKey->Render();
}
