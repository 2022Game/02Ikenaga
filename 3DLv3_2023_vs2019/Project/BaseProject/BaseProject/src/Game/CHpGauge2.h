#pragma once
#include "CObjectBase.h"
#include "CColliderSphere.h"
#include "CBillBoardImage.h"
class CImage;

/*
敵のHPクラス
オブジェクトベース継承
*/
class CHpGauge2 : public CBillBoardImage
{
private:
	CImage* mpFrameImage;  //ゲージのフレームのイメージ
	CImage* mpBarImage;    //ゲージのバーのイメージ	
	int mMaxValue;         //ポイントの最大値
	int mValue;            //ポイン

public:
	CHpGauge2();
	~CHpGauge2();

	// 初期設定
	void Setup(const CVector& pos, const CVector& dir);

	//最大値を設定
	void SetMaxValue(int value);
	//現在値を設定
	void SetValue(int value);


	void Update() override;
};