#pragma once
#include "CTask.h"
#include "CHpGauge.h"
#include "CLevelUI.h"

// ゲーム中のエネミーUI
class CGameEnemyUI : public CTask
{
public:

	// コンストラク
	CGameEnemyUI();

	// デストラクタ
	~CGameEnemyUI();

	// HPを設定
	void SetHp(int hp);
	// 最大のHPを設定
	void SetMaxHp(int maxHp);
	// HPゲージを取得
	CHpGauge* GetHpGauge() const;
	// HPゲージのオフセット位置
	void SetHpGaugeOffsetPos(const CVector& pos);
	// HPゲージのオフセット位置を取得
	const CVector& GetHpGaugeOffsetPos() const;

	// Lvを設定
	void SetLv(int lv);
	// Lvを取得
	CLevelUI* GetLv()const;
	// Lvのオフセット位置
	void SetLvOffsetPos(const CVector& pos);
	// Lvのオフセット位置を取得
	const CVector& GetLvOffsetPos() const;

private:
	// UI関連
	CVector mpHpGaugeOffsetPos;  // HPゲージのオフセット位置
	CHpGauge* mpHpGauge;    // HPゲージ
	CLevelUI* mpLvUI;       // レベルのUI(Lv)
	CVector mpLvOffsetPos;  // Lvのオフセット位置
};