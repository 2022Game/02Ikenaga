#pragma once
#include "CUIBase.h"
#include "CVector.h"
class CImage;

/*
 回避ゲージ
*/
class CAvoidanceGauge : public CUIBase
{
public:

	// コンストラクタ
	CAvoidanceGauge(bool is3dGauge);

	// デストラクタ
	~CAvoidanceGauge();

	// 回避ゲージを削除
	void Kill()override;

	// 表示するかどうか設定
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

private:

	CImage* mpFrameImage;  // ゲージのフレームのイメージ
	CImage* mpBarImage;    // ゲージのバーのイメージ	
	CImage* mpEdgeImage;   // ゲージのふち
	int mMaxValue;         // ポイントの最大値
	int mValue;            // ポイントの現在値
	CVector2 mCenterRatio; // 中心位置の割合
	float mScale;          // 回避ゲージのスケール値
	bool mIs3dGauge;       // 3D空間に配置するゲージかどうか
};