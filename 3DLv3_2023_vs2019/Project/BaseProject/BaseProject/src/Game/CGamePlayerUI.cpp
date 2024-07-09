#include "CGamePlayerUI.h"
#include "CPlayerNameUI.h"
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

// AvoidGaugeを取得
CAvoidanceGauge* CGamePlayerUI::GetAvoidGauge() const
{
	return mpAvoidGauge;
}