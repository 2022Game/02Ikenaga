#pragma once
#include "CBillBoardImage.h"

// ボスUI
class CBossUI : public CBillBoardImage
{
public:
	// コンストラクタ
	CBossUI(float height);

	// デストラクタ
	~CBossUI();

	// 持ち主を設定
	void SetOwner(CCharaBase* owner) override;

	// 更新
	void Update() override;

private:

	float mBaseScale;    // 持ち主のベーススケール値
	float mHeight;       // 高さ
};