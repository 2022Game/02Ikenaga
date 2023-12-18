#pragma once
#include "CUIBase.h"
class CBillBoardImage;

/*
敵のHPクラス
オブジェクトベース継承
*/
class CHpGauge2 : public CUIBase
{
private:
	CBillBoardImage* mpBarImage;    //ゲージのバーのイメージ	
	CBillBoardImage* mpFrameImage;  //ゲージのフレームのイメージ
	int mMaxValue;         //ポイントの最大値
	int mValue;            //ポイン
	CVector mMoveSpeed;  //移動速度

public:
	CHpGauge2();
	~CHpGauge2();

	// 初期設定
	void Setup(const CVector& pos, const CVector& dir);

	//最大値を設定
	void SetMaxValue(int value);
	//現在値を設定
	void SetValue(int value);

	void Update()override;
};