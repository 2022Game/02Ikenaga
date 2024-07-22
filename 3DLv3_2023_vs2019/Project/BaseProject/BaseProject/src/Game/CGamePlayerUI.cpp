#include "CGamePlayerUI.h"
#include "CPlayerNameUI.h"
#include "CLevelUI.h"
#include "CPlayerLevelUI.h"
#include "CHpUI.h"
#include "CSpUI.h"
#include "CExpUI.h"
#include "CHpGauge.h"
#include "CSpGauge.h"
#include "CAvoidanceGauge.h"
#include "CExpGauge.h"

// コンストラク
CGamePlayerUI::CGamePlayerUI()
	:CTask(ETaskPriority::eUI,0,ETaskPauseType::eGame)
{
	// プレイヤー名のUIを作成
	mpNameUI = new CPlayerNameUI();

	// Lv.UIを作成
	mpLevelUI = new CLevelUI(false);
	// プレイヤーレベルのUIを作成
	mpPlayerLevelUI = new CPlayerLevelUI(50.0f,32.0f);
	mpPlayerLevelUI->SetShow(true);

	// HpUIを作成
	mpHpUI = new CHpUI();

	// SpUIを作成
	mpSpUI = new CSpUI();

	// ExpUIを作成
	mpExpUI = new CExpUI();

	// プレイヤーのHPゲージを作成
	mpHpGauge = new CHpGauge(false);
	mpHpGauge->SetPos(10.0f, 63.0f);
	mpHpGauge->SetShow(true);

	// プレイヤーのSPゲージを作成
	mpSpGauge = new CSpGauge();
	mpSpGauge->SetPos(10.0f, 103.5f);

	// プレイヤーの回避ゲージ作成
	mpAvoidGauge = new CAvoidanceGauge(true);
	mpAvoidGauge->SetCenterRatio(CVector2(0.5f, 0.0f));

	// プレイヤーのExpゲージ作成
	mpExpGauge = new CExpGauge();
	mpExpGauge->SetPos(320.0f, 700.0f);
}

// デストラクタ
CGamePlayerUI::~CGamePlayerUI()
{
	// UI関連の削除
	mpNameUI->Kill();
	mpLevelUI->Kill();
	mpPlayerLevelUI->Kill();
	mpHpUI->Kill();
	mpSpUI->Kill();
	mpExpUI->Kill();
	// ゲージ関連の削除
	mpHpGauge->Kill();
	mpSpGauge->Kill();
	mpAvoidGauge->Kill();
	mpExpGauge->Kill();
}

// プレイヤー名を設定
void CGamePlayerUI::SetPlayerName(std::string name)
{
	mpNameUI->SetPlayerName(name);
}

// Lvを設定
void CGamePlayerUI::SetLevel(std::string lv)
{
	mpLevelUI->SetLevel(lv);
}

// プレイヤーレベルを設定
void CGamePlayerUI::SetPlayerLevel(int level)
{
	mpPlayerLevelUI->SetLevel(level);
}

// 最大Hpを設定
void CGamePlayerUI::SetMaxHp(int maxHp)
{
	mpHpGauge->SetMaxValue(maxHp);
}

// 現在のHpを設定
void CGamePlayerUI::SetHp(int hp)
{
	mpHpGauge->SetValue(hp);
}

// Hpを表示
void CGamePlayerUI::SetHpText(std::string hpText)
{
	mpHpUI->SetHpText(hpText);
}

// 最大Spを設定
void CGamePlayerUI::SetMaxSp(int maxSp)
{
	mpSpGauge->SetMaxValue(maxSp);
}

// 現在のSpを設定
void CGamePlayerUI::SetSp(int sp)
{
	mpSpGauge->SetValue(sp);
}

// Spを表示
void CGamePlayerUI::SetSpText(std::string spText)
{
	mpSpUI->SetSpText(spText);
}

// 最大Avoidを設定
void CGamePlayerUI::SetAvoidMaxValue(int maxValue)
{
	mpAvoidGauge->SetMaxValue(maxValue);
}

// 現在のAvoidを設定
void CGamePlayerUI::SetAvoidValue(int value)
{
	mpAvoidGauge->SetValue(value);
}

// 最大Expを設定
void CGamePlayerUI::SetMaxExp(int maxExp)
{
	mpExpGauge->SetMaxValue(maxExp);
}

// 現在のExpを設定
void CGamePlayerUI::SetExp(int exp)
{
	mpExpGauge->SetValue(exp);
}

// Expを表示
void CGamePlayerUI::SetExpText(std::string expText)
{
	mpExpUI->SetExpText(expText);
}

// AvoidGaugeを取得
CAvoidanceGauge* CGamePlayerUI::GetAvoidGauge() const
{
	return mpAvoidGauge;
}