#pragma once
#include "CTask.h"

class CPlayerNameUI;
class CLevelUI;
class CPlayerLevelUI;
class CHpGauge;
class CSpGauge;
class CAvoidanceGauge;
class CExpGauge;
class CHpUI;
class CSpUI;
class CExpUI;
class CLockLevel;
class CLockLevel2;

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

	// Lvを設定
	void SetLevel(std::string lv);
	// プレイヤーレベルを設定
	void SetPlayerLevel(int level);

	// 最大Hpを設定
	void SetMaxHp(int maxHp);
	// 現在のHpを設定
	void SetHp(int hp);
	// Hpを表示
	void SetHpText(std::string hpText);

	// 最大Spを設定
	void SetMaxSp(int maxSp);
	// 現在のSpを設定
	void SetSp(int sp);
	// Spを表示
	void SetSpText(std::string spText);

	// 最大Avoidを設定
	void SetAvoidMaxValue(int maxValue);
	// 現在のAvoidを設定
	void SetAvoidValue(int value);

	// 最大Expを設定
	void SetMaxExp(int maxExp);
	// 現在のExpを設定
	void SetExp(int exp);
	// Expを表示
	void SetExpText(std::string expText);

	// ロック中のレベル
	void SetLockLevel(std::string lockLevel);
	// ロック中のレベル2
	void SetLockLevel2(std::string lockLevel2);

	// AvoidGaugeを取得
	CAvoidanceGauge* GetAvoidGauge()const;

private:
	// UI関連
	CPlayerNameUI* mpNameUI;          // 名前のUI
	CLevelUI* mpLevelUI;              // レベルのUI(Lv)
	CPlayerLevelUI* mpPlayerLevelUI;  // プレイヤーのレベルUI
	CHpUI* mpHpUI;                    // HpのUI
	CSpUI* mpSpUI;                    // SpのUI
	CExpUI* mpExpUI;                  // ExpのUI
	CLockLevel* mpLockLevel;          // ロック中のレベル
	CLockLevel2* mpLockLevel2;        // ロック中のレベル

	// ゲージ関連
	CHpGauge* mpHpGauge;            // Hpゲージ
	CSpGauge* mpSpGauge;            // Spゲージ
	CAvoidanceGauge* mpAvoidGauge;  // 回避ゲージ
	CExpGauge* mpExpGauge;          // Expゲージ
};