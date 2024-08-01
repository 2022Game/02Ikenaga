#pragma once
#include "CTask.h"
#include "CHpGauge.h"
#include "CLevelUI.h"
#include "CEnemyLevelUI.h"
#include "CEnemyNameUI.h"

// ゲーム中のエネミーUI
class CGameEnemyUI : public CTask
{
public:

	// コンストラク
	CGameEnemyUI();

	// デストラクタ
	~CGameEnemyUI();

	// UIのオフセット位置
	void SetUIoffSetPos(const CVector& pos);
	// UIのオフセット位置を取得
	const CVector& GetUIoffSetPos() const;

	// 最大の大きさを設定
	void SetMaxScale(float maxScale);
	// 最小の大きさを設定
	void SetMinScale(float minScale);

	// HPを設定
	void SetHp(int hp);
	// 最大のHPを設定
	void SetMaxHp(int maxHp);
	// HPゲージを取得
	CHpGauge* GetHpGauge() const;

	// Lvを設定
	void SetLv();
	// Lvを取得
	CLevelUI* GetLv() const;

	// レベルを設定
	void SetEnemyLevel(int level);
	// レベルを取得
	CEnemyLevelUI* GetLevel() const;

	// 名前を設定
	void SetEnemyName(std::string name);
	// 名前を取得
	CEnemyNameUI* GetName() const;

private:
	// UI関連
	CVector mpUIoffSetPos;     // UIのオフセット位置
	CHpGauge* mpHpGauge;       // HPゲージ
	CLevelUI* mpLvUI;          // レベルのUI(Lv)
	CEnemyLevelUI* mpLevelUI;  // 敵のレベルUI(数字)
	CEnemyNameUI* mpNameUI;    // 敵の名前
};