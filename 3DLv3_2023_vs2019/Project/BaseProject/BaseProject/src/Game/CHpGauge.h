#pragma once
#include "CUIBase.h"
#include "CVector.h"
class CImage;

/*
HPゲージ
*/
class CHpGauge : public CUIBase
{
private:
	CImage* mpFrameImage;  // ゲージのフレームのイメージ
	CImage* mpBarImage;    // ゲージのバーのイメージ	
	CImage* mpEdgeImage;   // ゲージのふち
	int mMaxValue;         // ポイントの最大値
	int mValue;            // ポイントの現在値
	CVector2 mCenterRatio; // 中心位置の割合
	float mScale;          // HPゲージのスケール値
	bool mIs3dGauge;       // #D空間に配置するゲージかどうか

public:
	// コンストラクタ
	CHpGauge(bool is3dGauge);
	// デストラクタ
	~CHpGauge();

	// HPゲージを削除
	void Kill()override;

	//表示するかどうか設定
	void SetShow(bool isShow)override;

	// 最大値を設定
	void SetMaxValue(int value);
	// 現在値を設定
	void SetValue(int value);

	// 中心位置のの割合の設定
	void SetCenterRatio(const CVector2& ratio);

	// ワールド座標を設定
	void SetWorldPos(const CVector& worldPos);

	// 更新
	void Update();
};