#pragma once
#include "CUIBase.h"
class CImage;

/*
HPゲージ
*/
class CHpGauge : public CUIBase
{
private:
	CImage* mpFrameImage;  //ゲージのフレームのイメージ
	CImage* mpBarImage;    //ゲージのバーのイメージ	
	int mMaxValue;         //ポイントの最大値
	int mValue;            //ポイントの現在値

public:
	//コンストラクタ
	CHpGauge();
	//デストラクタ
	~CHpGauge();

	//最大値を設定
	void SetMaxValue(int value);
	//現在値を設定
	void SetValue(int value);

	//更新
	void Update();
};