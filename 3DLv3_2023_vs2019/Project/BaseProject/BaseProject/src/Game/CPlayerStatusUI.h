#pragma once
#include "CTask.h"
#include "CImage.h"

class CText;
class CPlayerLevelUI;
class CPlayerMaxExpUI;
class CPlayerExpUI;
class CPlayerMaxHpUI;
class CPlayerHpUI;
class CPlayerMaxSpUI;
class CPlayerSpUI;
class CPlayerPowerUI;
class CPlayerDefenseUI;
class CPlayerScaleUI;

// プレイヤーのステータスUI
class CPlayerStatusUI : public CTask
{
public:
	// コンストラク
	CPlayerStatusUI();
	// デストラクタ
	~CPlayerStatusUI();

	// 表示設定
	void SetShow(bool show)override;

	// オープン
	void Open();
	// クローズ
	void Close();
	// オープンするかどうか
	bool IsOpened() const;

	// 決める
	void Decide(int select);

	// プレイヤーのレベルを設定
	void SetLevel(int level);

	// プレイヤーの最大経験値を設定
	void SetMaxExp(int maxExp);
	// プレイヤーの経験値を設定
	void SetExp(int exp);

	// プレイヤーの最大HPを設定
	void SetMaxHp(int maxHp);
	// プレイヤーのHPを設定
	void SetHp(int hp);

	// プレイヤーの最大SPを設定
	void SetMaxSp(int maxSp);
	// プレイヤーのSPを設定
	void SetSp(int sp);

	// プレイヤーの攻撃力を設定
	void SetPower(int power);

	// プレイヤーの防御力を設定
	void SetDefense(int defense);

	// プレイヤーの大きさを設定
	void SetScale(float scale);

	// 更新
	void Update() override;
	// 描画
	void Render() override;

private:
	// 現在のプレイヤーのステータスを反映
	void ApplyPlayerStatus();

	// 背景
	CImage* mpBackground;
	// ステータス
	CImage* mpStatus;
	// 矢印
	CImage* mpBack;
	// キー
	CImage* mpShiftKey;
	// バフスキル
	CImage* mpBuffSkill;
	// スラッシュスキル
	CImage* mpSlashSkill;

	// テキスト関連
	CText* mpLevelText;   // レベルのテキスト
	CText* mpExpText;     // 経験値のテキスト
	CText* mpHpText;      // HPのテキスト
	CText* mpSpText;      // SPのテキスト
	CText* mpAttackText;  // 攻撃力のテキスト
	CText* mpDefenseText; // 防御力のテキスト
	CText* mpSizeText;    // 大きさのテキスト
	CText* mpLine;        // 線
	CText* mpLine2;       // 線2
	CText* mpLine3;       // 線3
	CText* mpDetail;      // 詳細
	CText* mpSkillText;   // スキル
	CText* mpSkillText2;   // スキル2
	// UI関連
	CPlayerLevelUI* mpLevelUI;     // レベル
	CPlayerMaxExpUI* mpMaxExpUI;   // 経験値
	CPlayerExpUI* mpExpUI;         // 経験値
	CPlayerMaxHpUI* mpMaxHpUI;     // 最大Hp
	CPlayerHpUI* mpHpUI;           // Hp
	CPlayerMaxSpUI* mpMaxSpUI;     // 最大Sp
	CPlayerSpUI* mpSpUI;           // Sp
	CPlayerPowerUI* mpPowerUI;     // 攻撃力
	CPlayerDefenseUI* mpDefenseUI; // 防御力
	CPlayerScaleUI* mpScaleUI;     // 大きさ
	bool mIsOpened;
};