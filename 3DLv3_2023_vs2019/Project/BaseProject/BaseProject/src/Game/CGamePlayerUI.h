#pragma once
#include "CTask.h"

class CPlayerNameUI;
class CHpGauge;
class CSpGauge;
class CAvoidanceGauge;
class CExpGauge;

// ゲーム中のプレイヤーUI
class CGamePlayerUI : public CTask
{
public:

	// コンストラク
	CGamePlayerUI();

	// デストラクタ
	~CGamePlayerUI();

	// プレイヤー名を設定
	void SetPlayerName(std::string name);

	// 最大Hpを設定
	void SetMaxHp(int maxHp);
	// 現在のHpを設定
	void SetHp(int hp);

	// 最大Spを設定
	void SetMaxSp(int maxSp);
	// 現在のSpを設定
	void SetSp(int sp);

	// 最大Avoidを設定
	void SetAvoidMaxValue(int maxValue);
	// 現在のAvoidを設定
	void SetAvoidValue(int value);

	// 最大Expを設定
	void SetMaxExp(int maxExp);
	// 現在のExpを設定
	void SetExp(int exp);

	// AvoidGaugeを取得
	CAvoidanceGauge* GetAvoidGauge()const;

private:
	CPlayerNameUI* mpNameUI;
	CHpGauge* mpHpGauge;
	CSpGauge* mpSpGauge;
	CAvoidanceGauge* mpAvoidGauge;
	CExpGauge* mpExpGauge;
};