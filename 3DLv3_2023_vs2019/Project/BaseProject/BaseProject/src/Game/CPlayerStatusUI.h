#pragma once
#include "CTask.h"
#include "CImage.h"

class CText;
class CPlayerLevelUI;
class CPlayerExpUI;
class CPlayerMaxHpUI;
class CPlayerHpUI;

// プレイヤーのステータスUI
class CPlayerStatusUI : public CTask
{
public:
	// コンストラク
	CPlayerStatusUI();
	// デストラクタ
	~CPlayerStatusUI();

	// オープン
	void Open();
	// クローズ
	void Close();
	// オープンするかどうか
	bool IsOpened() const;

	// 決める
	void Decide(int select);

	// プレイヤーレベルを設定
	void SetLevel(int level);

	// プレイヤー経験値を設定
	void SetExp(int exp);

	// プレイヤー最大HPを設定
	void SetMaxHp(int maxHp);
	// プレイヤーHPを設定
	void SetHp(int hp);

	// 更新
	void Update() override;
	// 描画
	void Render() override;

private:
	// 背景
	CImage* mpBackground;
	// ステータス
	CImage* mpStatus;
	// テキスト関連
	CText* mpLevelText;   // レベルのテキスト
	CText* mpExpText;     // 経験値のテキスト
	CText* mpHpText;      // HPのテキスト
	CText* mpSpText;      // SPのテキスト
	CText* mpAttackText;  // 攻撃力のテキスト
	CText* mpDefenseText; // 防御力のテキスト
	CText* mpSizeText;    // 大きさのテキスト
	CText* mpLine2;       // 線2
	CText* mpLine3;       // 線3
	// UI関連
	CPlayerLevelUI* mpLevelUI;  // レベルUI
	CPlayerExpUI* mpExpUI;      // 経験値
	CPlayerMaxHpUI* mpMaxHpUI;  // 最大HpUI
	CPlayerHpUI* mpHpUI;        // HpUI
	bool mIsOpened;
};