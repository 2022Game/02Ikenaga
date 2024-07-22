#pragma once
#include "CTask.h"
#include "CImage.h"

class CText;

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

	// Levelを設定
	void SetLevel(std::string level);

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

	bool mIsOpened;
};