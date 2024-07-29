#pragma once
#include "CUIBase.h"
class CImage;

/*
 経験値のゲージ
*/
class CExpGauge : public CUIBase
{
public:

	// コンストラクタ
	CExpGauge();

	// デストラクタ
	~CExpGauge();

	// 最大値を設定
	void SetMaxValue(int value);

	// 現在値を設定
	void SetValue(int value);

	// 表示するかどうか設定
	void SetShow(bool isShow)override;

	// 更新
	void Update();

private:

	CImage* mpBarImage;    // ゲージのバー
	CImage* mpEdgeImage;   // ゲージのふち
	int mMaxValue;         // ポイントの最大値
	int mValue;            // ポイントの現在値
};