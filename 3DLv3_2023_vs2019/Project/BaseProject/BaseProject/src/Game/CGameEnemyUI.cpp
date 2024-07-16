#include "CGameEnemyUI.h"

// コンストラク
CGameEnemyUI::CGameEnemyUI()
	:CTask(ETaskPriority::eUI, 0, ETaskPauseType::eGame)
{
	// HPゲージ作成
	mpHpGauge = new CHpGauge(true);
	mpHpGauge->SetCenterRatio(CVector2(0.5f, 0.0f));

	// Lv.UIを作成
	mpLvUI = new CLevelUI(true);
	mpLvUI->SetCenterRatio(CVector2(0.5f, 0.0f));
}

// デストラクタ
CGameEnemyUI::~CGameEnemyUI()
{
	// HPゲージの削除
	mpHpGauge->Kill();
	// LvUIの削除
	mpLvUI->Kill();
}

// HPを設定
void CGameEnemyUI::SetHp(int hp)
{
	mpHpGauge->SetValue(hp);
}

// 最大HPを設定
void CGameEnemyUI::SetMaxHp(int maxHp)
{
	mpHpGauge->SetMaxValue(maxHp);
}

// HPゲージのオフセット位置
void CGameEnemyUI::SetHpGaugeOffsetPos(const CVector& pos)
{
	mpHpGaugeOffsetPos = pos;
}

// UIのオフセット位置を取得
const CVector& CGameEnemyUI::GetHpGaugeOffsetPos() const
{
	return mpHpGaugeOffsetPos;
}

// HPゲージを取得
CHpGauge* CGameEnemyUI::GetHpGauge() const
{
	return mpHpGauge;
}

// Lvを設定
void CGameEnemyUI::SetLv(int lv)
{
	std::string lvStr = "Lv." + std::to_string(lv);
	mpLvUI->SetLevel(lvStr);
}

// Lvを取得
CLevelUI* CGameEnemyUI::GetLv()const
{
	return mpLvUI;
}

// Lvのオフセット位置
void CGameEnemyUI::SetLvOffsetPos(const CVector& pos)
{
	mpLvOffsetPos = pos;
}

// UIのオフセット位置を取得
const CVector& CGameEnemyUI::GetLvOffsetPos() const
{
	return mpLvOffsetPos;
}