#include "CPlayerStatusUI.h"
#include "CInput.h"
#include "CTaskManager.h"
#include "CBGMManager.h"
#include "CGameMenu.h"
#include "CText.h"
#include "CPlayer.h"
#include "CPlayerLevelUI.h"
#include "CPlayerMaxExpUI.h"
#include "CPlayerExpUI.h"
#include "CPlayerMaxHpUI.h"
#include "CPlayerHpUI.h"
#include "CPlayerMaxSpUI.h"
#include "CPlayerSpUI.h"
#include "CPlayerPowerUI.h"
#include "CPlayerDefenseUI.h"
#include "CPlayerScaleUI.h"

#define LV_POS CVector2(220.0f, 180.0f)
#define EXP_POS CVector2(210.0f, 220.0f)
#define HP_POS CVector2(220.0f, 260.0f)
#define SP_POS CVector2(220.0f, 300.0f)
#define ATTACK_POS CVector2(200.0f, 340.0f)
#define DEFENSE_POS CVector2(200.0f, 380.0f)
#define SIZE_POS CVector2(200.0f, 420.0f)
#define DETAIL_POS CVector2(200.0f, 500.0f)

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

	mpBack = new CImage
	(
		"UI/back.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpBack->SetPos(140.0f, 600.0f);
	mpBack->SetSize(30.0f, 30.0f);

	mpShiftKey = new CImage
	(
		"UI/Key/shift.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpShiftKey->SetPos(185.0f, 590.0f);
	mpShiftKey->SetSize(55.0f, 55.0f);

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
		ETaskPauseType::eMenu,
		false, false
	);
	mpLevelText->SetTextAlignH(textAlignH);
	mpLevelText->SetText("Lv.");

	// 経験値のテキストを作成
	mpExpText = new CText
	(
		nullptr, 24,
		EXP_POS,
		size,
		CColor(1.0f, 1.0f, 1.0f),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eMenu,
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
		ETaskPauseType::eMenu,
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
		ETaskPauseType::eMenu,
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
		ETaskPauseType::eMenu,
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
		ETaskPauseType::eMenu,
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
		ETaskPauseType::eMenu,
		false, false
	);
	mpSizeText->SetTextAlignH(textAlignH);
	mpSizeText->SetText("大きさ");

	// 線を作成
	mpLine = new CText
	(
		nullptr, 24,
		CVector2(410.0f, 220.0f),
		size,
		CColor(1.0f, 1.0f, 1.0f),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eMenu,
		false, false
	);
	mpLine->SetTextAlignH(textAlignH);
	mpLine->SetText("/");

	// 線2を作成
	mpLine2 = new CText
	(
		nullptr, 24,
		CVector2(410.0f,260.0f),
		size,
		CColor(1.0f, 1.0f, 1.0f),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eMenu,
		false, false
	);
	mpLine2->SetTextAlignH(textAlignH);
	mpLine2->SetText("/");

	// 線3を作成
	mpLine3 = new CText
	(
		nullptr, 24,
		CVector2(410.0f, 300.0f),
		size,
		CColor(1.0f, 1.0f, 1.0f),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eMenu,
		false, false
	);
	mpLine3->SetTextAlignH(textAlignH);
	mpLine3->SetText("/");

	// 詳細のテキストを作成
	mpDetail = new CText
	(
		nullptr, 24,
		DETAIL_POS,
		size,
		CColor(1.0f, 1.0f, 1.0f),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eMenu,
		false, false
	);
	mpDetail->SetTextAlignH(textAlignH);
	mpDetail->SetText("剣と盾で様々な困難を乗り越えスキルで自分を強化、回復をしその身を極めていく。");

	// レベルUIを作成
	mpLevelUI = new CPlayerLevelUI(400.0f, 180.f);

	// 最大経験値のUIを作成
	mpMaxExpUI = new CPlayerMaxExpUI(455.0f, 220.f);

	// 経験値のUIを作成
	mpExpUI = new CPlayerExpUI(350.0f, 220.f);

	// 最大HPのUIを作成
	mpMaxHpUI = new CPlayerMaxHpUI(445.0f, 260.f);

	// HPのUIを作成
	mpHpUI = new CPlayerHpUI(340.0f, 260.f);

	// 最大SPのUIを作成
	mpMaxSpUI = new CPlayerMaxSpUI(455.0f, 300.f);

	// SPのUIを作成
	mpSpUI = new CPlayerSpUI(350.0f, 300.f);

	// 攻撃力のUIを作成
	mpPowerUI = new CPlayerPowerUI(400.0f,340.0f);

	// 防御力のUIを作成
	mpDefenseUI = new CPlayerDefenseUI(400.0f, 380.0f);

	// 大きさのUIを作成
	mpScaleUI = new CPlayerScaleUI(360.0f, 420.0f);

	SetEnable(false);
	SetShow(false);
}

// デストラクタ
CPlayerStatusUI::~CPlayerStatusUI()
{
	SAFE_DELETE(mpBackground);
	SAFE_DELETE(mpStatus);
	SAFE_DELETE(mpBack);
	SAFE_DELETE(mpShiftKey);
	SAFE_DELETE(mpLevelText);
	SAFE_DELETE(mpExpText);
	SAFE_DELETE(mpHpText);
	SAFE_DELETE(mpSpText);
	SAFE_DELETE(mpAttackText);
	SAFE_DELETE(mpDefenseText);
	SAFE_DELETE(mpSizeText);
	SAFE_DELETE(mpLine);
	SAFE_DELETE(mpLine2);
	SAFE_DELETE(mpLine3);
	mpLevelUI->Kill();
	mpMaxExpUI->Kill();
	mpExpUI->Kill();
	mpMaxHpUI->Kill();
	mpHpUI->Kill();
	mpMaxSpUI->Kill();
	mpSpUI->Kill();
	mpPowerUI->Kill();
	mpDefenseUI->Kill();
	mpScaleUI->Kill();
}

// 表示設定
void CPlayerStatusUI::SetShow(bool show)
{
	CTask::SetShow(show);

	mpLevelUI->SetShow(show);
	mpMaxExpUI->SetShow(show);
	mpExpUI->SetShow(show);
	mpMaxHpUI->SetShow(show);
	mpHpUI->SetShow(show);
	mpMaxSpUI->SetShow(show);
	mpSpUI->SetShow(show);
	mpPowerUI->SetShow(show);
	mpDefenseUI->SetShow(show);
	mpScaleUI->SetShow(show);
}

// 現在のプレイヤーのステータスを反映
void CPlayerStatusUI::ApplyPlayerStatus()
{
	CPlayer* player = CPlayer::Instance();
	if (player == nullptr)return;

	const CharaStatus& status = player->Status();
	const CharaStatus& maxStatus = player->MaxStatus();

	// 最大ステータスを反映
	SetMaxHp(maxStatus.hp);
	SetMaxSp(maxStatus.SpecialPoint);
	SetMaxExp(maxStatus.exp);

	// 現在のステータスを反映
	SetHp(status.hp);
	SetSp(status.SpecialPoint);
	SetExp(status.exp);
	SetPower(status.power);
	SetDefense(status.defense);
	SetScale(status.volume);
	SetLevel(status.level);
}

// オープン
void CPlayerStatusUI::Open()
{
	SetEnable(true);
	SetShow(true);
	mIsOpened = true;

	ApplyPlayerStatus();
}

// クローズ
void CPlayerStatusUI::Close()
{
	SetEnable(false);
	SetShow(false);

	CGameMenu::Instance()->Open();
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

// プレイヤーのレベルを設定
void CPlayerStatusUI::SetLevel(int level)
{
	mpLevelUI->SetLevel(level);
}

// プレイヤーの最大経験値を設定
void CPlayerStatusUI::SetMaxExp(int maxExp)
{
	mpMaxExpUI->SetMaxExp(maxExp);
}

// プレイヤーの経験値を設定
void CPlayerStatusUI::SetExp(int exp)
{
	mpExpUI->SetExp(exp);
}

// プレイヤーの最大HPを設定
void CPlayerStatusUI::SetMaxHp(int maxHp)
{
	mpMaxHpUI->SetMaxHp(maxHp);
}

// プレイヤーのHPを設定
void CPlayerStatusUI::SetHp(int hp)
{
	mpHpUI->SetHp(hp);
}

// プレイヤーの最大SPを設定
void CPlayerStatusUI::SetMaxSp(int maxSp)
{
	mpMaxSpUI->SetMaxSp(maxSp);
}

// プレイヤーのSPを設定
void CPlayerStatusUI::SetSp(int sp)
{
	mpSpUI->SetSp(sp);
}

// プレイヤーの攻撃力を設定
void CPlayerStatusUI::SetPower (int power)
{
	mpPowerUI->SetPower(power);
}

// プレイヤーの防御力を設定
void CPlayerStatusUI::SetDefense(int defense)
{
	mpDefenseUI->SetDefense(defense);
}

// プレイヤーの大きさを設定
void CPlayerStatusUI::SetScale(float scale)
{
	mpScaleUI->SetScale(scale);
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
	mpBack->Render();
	mpShiftKey->Render();

	mpLevelText->SetColor(1.0f, 1.0f, 1.0f);
	mpExpText->SetColor(1.0f, 1.0f, 0.0f);
	mpHpText->SetColor(0.0f, 1.0f, 0.0f);
	mpSpText->SetColor(1.0f, 0.0f, 0.8f);
	mpAttackText->SetColor(1.0f, 0.0f, 0.0f);
	mpDefenseText->SetColor(0.0f, 1.0f, 1.0f);
	mpSizeText->SetColor(1.0f, 0.5f, 0.0f);
	mpLine->SetColor(1.0f, 1.0f, 1.0f);
	mpLine2->SetColor(1.0f, 1.0f, 1.0f);
	mpLine3->SetColor(1.0f, 1.0f, 1.0f);
	mpDetail->SetColor(1.0f, 1.0f, 1.0f);

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

		// 線
		mpLine->SetPos(CVector2(410.0f, 220.0f) + (CVector2)(rot * v) * SHADOW_WIDTH);
		mpLine->Render();

		// 線2
		mpLine2->SetPos(CVector2(410.0f,260.0f) + (CVector2)(rot * v) * SHADOW_WIDTH);
		mpLine2->Render();

		// 線3
		mpLine3->SetPos(CVector2(410.0f, 300.0f) + (CVector2)(rot * v) * SHADOW_WIDTH);
		mpLine3->Render();

		// 詳細
		mpDetail->SetPos(DETAIL_POS + (CVector2)(rot * v) * SHADOW_WIDTH);
		mpDetail->Render();
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

	mpLine->SetColor(0.0f, 0.0f, 0.0f);
	mpLine->SetPos(CVector2(410.0f, 220.0f));
	mpLine->Render();

	mpLine2->SetColor(0.0f, 0.0f, 0.0f);
	mpLine2->SetPos(CVector2(410.0f, 260.0f));
	mpLine2->Render();

	mpLine3->SetColor(0.0f, 0.0f, 0.0f);
	mpLine3->SetPos(CVector2(410.0f, 300.0f));
	mpLine3->Render();

	mpDetail->SetColor(0.0f, 0.0f, 0.0f);
	mpDetail->SetPos(DETAIL_POS);
	mpDetail->Render();
}