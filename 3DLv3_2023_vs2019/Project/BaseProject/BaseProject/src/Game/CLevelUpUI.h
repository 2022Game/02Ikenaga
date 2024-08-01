#pragma once
#include "CBillBoardImage.h"

// レベルアップUI
class CLevelUpUI : public CBillBoardImage
{
public:
	// コンストラクタ
	CLevelUpUI(float height);

	// デストラクタ
	~CLevelUpUI();

	// 持ち主を設定
	void SetOwner(CCharaBase* owner) override;

	// 開始
	void StartLevelUpUI();
	// 終了
	void EndLevelUpUI();

	// 更新
	void Update() override;

private:

	// 自身のベーススケール値を算出
	float CalcScale() const;

	float mElapsedTime;  // 経過時間
	float mBaseScale;    // 持ち主のベーススケール値
	float mHeight;       // 高さ
};