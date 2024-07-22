#include "CPlayerStatusUI.h"
#include "CInput.h"
#include "CTaskManager.h"
#include "CBGMManager.h"
#include "CGameMenu.h"
#include "CText.h"

#define LV_POS CVector2(220.0f, 180.0f)
#define EXP_POS CVector2(210.0f, 220.0f)
#define HP_POS CVector2(220.0f, 260.0f)
#define SP_POS CVector2(220.0f, 300.0f)
#define ATTACK_POS CVector2(200.0f, 340.0f)
#define DEFENSE_POS CVector2(200.0f, 380.0f)
#define SIZE_POS CVector2(200.0f, 420.0f)

#define SHADOW_COUNT 8     // 影のカウント
#define SHADOW_WIDTH 2.0f  // 影の幅
#define MENU_ALPHA 0.75f

// コンストラ
CPlayerStatusUI::CPlayerStatusUI()
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

	mpStatus = new CImage
	(
		"UI/Text/status.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpStatus->SetPos(105.0f, 80.0f);
	mpStatus->SetSize(350.0f, 90.0f);

	CVector2 size = CVector2(WINDOW_WIDTH * 0.5, WINDOW_HEIGHT * 0.5f);
	ETextAlignH textAlignH = ETextAlignH::eLeft;

	// レベルのテキストを作成
	mpLevelText = new CText
	(
		nullptr, 24,
		LV_POS,
		size,
		CColor(1.0f, 1.0f, 1.0f),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpLevelText->SetTextAlignH(textAlignH);
	mpLevelText->SetText("Lv.");

	// 経験値のテキストを作成
	mpExpText = new CText
	(
		nullptr, 24,
		LV_POS,
		size,
		CColor(1.0f, 1.0f, 1.0f),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpExpText->SetTextAlignH(textAlignH);
	mpExpText->SetText("Exp");

	// HPのテキストを作成
	mpHpText = new CText
	(
		nullptr, 24,
		HP_POS,
		size,
		CColor(1.0f, 1.0f, 1.0f),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpHpText->SetTextAlignH(textAlignH);
	mpHpText->SetText("HP");

	// SPのテキストを作成
	mpSpText = new CText
	(
		nullptr, 24,
		SP_POS,
		size,
		CColor(1.0f, 1.0f, 1.0f),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpSpText->SetTextAlignH(textAlignH);
	mpSpText->SetText("SP");

	// 攻撃力のテキストを作成
	mpAttackText = new CText
	(
		nullptr, 24,
		ATTACK_POS,
		size,
		CColor(1.0f, 1.0f, 1.0f),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpAttackText->SetTextAlignH(textAlignH);
	mpAttackText->SetText("攻撃力");

	// 防御力のテキストを作成
	mpDefenseText = new CText
	(
		nullptr, 24,
		DEFENSE_POS,
		size,
		CColor(1.0f, 1.0f, 1.0f),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpDefenseText->SetTextAlignH(textAlignH);
	mpDefenseText->SetText("防御力");

	// 大きさのテキストを作成
	mpSizeText = new CText
	(
		nullptr, 24,
		SIZE_POS,
		size,
		CColor(1.0f, 1.0f, 1.0f),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpSizeText->SetTextAlignH(textAlignH);
	mpSizeText->SetText("大きさ");

	SetEnable(false);
	SetShow(false);
}

// デストラクタ
CPlayerStatusUI::~CPlayerStatusUI()
{
	SAFE_DELETE(mpLevelText);
	SAFE_DELETE(mpExpText);
	SAFE_DELETE(mpHpText);
	SAFE_DELETE(mpSpText);
	SAFE_DELETE(mpAttackText);
	SAFE_DELETE(mpDefenseText);
	SAFE_DELETE(mpSizeText);
}

// オープン
void CPlayerStatusUI::Open()
{
	SetEnable(true);
	SetShow(true);
	mIsOpened = true;
	CBGMManager::Instance()->Play(EBGMType::eMenu, false);
	CTaskManager::Instance()->Pause(PAUSE_MENU_OPEN);
}

// クローズ
void CPlayerStatusUI::Close()
{
	SetEnable(false);
	SetShow(false);
	CBGMManager::Instance()->Play(EBGMType::eGame, false);
	CTaskManager::Instance()->UnPause(PAUSE_MENU_OPEN);
}

// オープンするかどうか
bool CPlayerStatusUI::IsOpened() const
{
	return mIsOpened;
}

// 決める
void CPlayerStatusUI::Decide(int select)
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

// Levelを設定
void CPlayerStatusUI::SetLevel(std::string level)
{
	mpLevelText->SetText(level.c_str());
}

// 更新
void CPlayerStatusUI::Update()
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
void CPlayerStatusUI::Render()
{
	mpBackground->Render();
	mpStatus->Render();

	mpLevelText->SetColor(1.0f, 1.0f, 1.0f);
	mpExpText->SetColor(1.0f, 1.0f, 1.0f);
	mpHpText->SetColor(1.0f, 1.0f, 1.0f);
	mpSpText->SetColor(1.0f, 1.0f, 1.0f);
	mpAttackText->SetColor(1.0f, 1.0f, 1.0f);
	mpDefenseText->SetColor(1.0f, 1.0f, 1.0f);
	mpSizeText->SetColor(1.0f, 1.0f, 1.0f);

	for (int i = 0; i < SHADOW_COUNT; i++)
	{
		CVector2 v = CVector2(1.0f, 0.0f);
		float alpha = (float)i / SHADOW_COUNT;
		CQuaternion rot = CQuaternion(0.0f, 0.0f, 360.0f * alpha);

		// レベル
		mpLevelText->SetPos(LV_POS + (CVector2)(rot * v) * SHADOW_WIDTH);
		mpLevelText->Render();

		// 経験値
		mpExpText->SetPos(EXP_POS + (CVector2)(rot * v) * SHADOW_WIDTH);
		mpExpText->Render();

		// HP
		mpHpText->SetPos(HP_POS + (CVector2)(rot * v) * SHADOW_WIDTH);
		mpHpText->Render();

		// SP
		mpSpText->SetPos(SP_POS + (CVector2)(rot * v) * SHADOW_WIDTH);
		mpSpText->Render();

		// 攻撃力
		mpAttackText->SetPos(ATTACK_POS + (CVector2)(rot * v) * SHADOW_WIDTH);
		mpAttackText->Render();

		// 防御力
		mpDefenseText->SetPos(DEFENSE_POS + (CVector2)(rot * v) * SHADOW_WIDTH);
		mpDefenseText->Render();

		// 大きさ
		mpSizeText->SetPos(SIZE_POS + (CVector2)(rot * v) * SHADOW_WIDTH);
		mpSizeText->Render();
	}
	mpLevelText->SetColor(0.0f, 0.0f, 0.0f);
	mpLevelText->SetPos(LV_POS);
	mpLevelText->Render();

	mpExpText->SetColor(0.0f, 0.0f, 0.0f);
	mpExpText->SetPos(EXP_POS);
	mpExpText->Render();

	mpHpText->SetColor(0.0f, 0.0f, 0.0f);
	mpHpText->SetPos(HP_POS);
	mpHpText->Render();

	mpSpText->SetColor(0.0f, 0.0f, 0.0f);
	mpSpText->SetPos(SP_POS);
	mpSpText->Render();

	mpAttackText->SetColor(0.0f, 0.0f, 0.0f);
	mpAttackText->SetPos(ATTACK_POS);
	mpAttackText->Render();

	mpDefenseText->SetColor(0.0f, 0.0f, 0.0f);
	mpDefenseText->SetPos(DEFENSE_POS);
	mpDefenseText->Render();

	mpSizeText->SetColor(0.0f, 0.0f, 0.0f);
	mpSizeText->SetPos(SIZE_POS);
	mpSizeText->Render();
}