#pragma once
#include "CUIBase.h"
class CImage;

/*
スペシャルポイントゲージ
*/
class CSpGauge : public CUIBase
{
private:
	CImage* mpFrameImage;  // ゲージのフレームのイメージ
	CImage* mpBarImage;    // ゲージのバーのイメージ	
	CImage* mpEdgeImage;   // ゲージのふち
	int mMaxValue;         // ポイントの最大値
	int mValue;            // ポイントの現在値

public:
	// コンストラクタ
	CSpGauge();

	// デストラクタ
	~CSpGauge();

	// 最大値を設定
	void SetMaxValue(int value);
	// 現在値を設定
	void SetValue(int value);

	// 更新
	void Update();
};
