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

	// Levelを作成
	mpLevelUI = new CEnemyLevelUI(true);
	mpLevelUI->SetCenterRatio(CVector2(0.5f, 0.0f));

	// 名前を作成
	mpNameUI = new CEnemyNameUI(true);
	mpNameUI->SetCenterRatio(CVector2(0.5f, 0.0f));
}

// デストラクタ
CGameEnemyUI::~CGameEnemyUI()
{
	// HPゲージの削除
	mpHpGauge->Kill();
	// LvUIの削除
	mpLvUI->Kill();
	// LevelUIの削除
	mpLevelUI->Kill();
	// 名前を削除
	mpNameUI->Kill();
}

// UIのオフセット位置
void CGameEnemyUI::SetUIoffSetPos(const CVector& pos)
{
	mpUIoffSetPos = pos;
}

// UIのオフセット位置を取得
const CVector& CGameEnemyUI::GetUIoffSetPos() const
{
	return mpUIoffSetPos;
}

// 大きさを設定
void CGameEnemyUI::SetScale(float scale)
{
	mpHpGauge->SetScale(scale);
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

// HPゲージを取得
CHpGauge* CGameEnemyUI::GetHpGauge() const
{
	return mpHpGauge;
}

// Lvを設定
void CGameEnemyUI::SetLv()
{
	std::string lvStr = "Lv.";
	mpLvUI->SetLevel(lvStr);
}

// Lvを取得
CLevelUI* CGameEnemyUI::GetLv()const
{
	return mpLvUI;
}

// レベルを設定
void CGameEnemyUI::SetEnemyLevel(int level)
{
	mpLevelUI->SetEnemyLevel(level);
}

// レベルを取得
CEnemyLevelUI* CGameEnemyUI::GetLevel() const
{
	return mpLevelUI;
}

// 名前を設定
void CGameEnemyUI::SetEnemyName(std::string name)
{
	mpNameUI->SetEnemyName(name);
}

// 名前を取得
CEnemyNameUI* CGameEnemyUI::GetName() const
{
	return mpNameUI;
}
