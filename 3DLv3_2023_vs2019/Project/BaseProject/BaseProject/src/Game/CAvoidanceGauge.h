#pragma once
#pragma once
#include "CUIBase.h"
#include "CVector.h"
class CImage;

/*
 回避ゲージ
*/
class CAvoidanceGauge : public CUIBase
{
private:
	CImage* mpFrameImage;  // ゲージのフレームのイメージ
	CImage* mpBarImage;    // ゲージのバーのイメージ	
	CImage* mpEdgeImage;   // ゲージのふち
	int mMaxValue;         // ポイントの最大値
	int mValue;            // ポイントの現在値
	CVector2 mCenterRatio; // 中心位置の割合
	float mScale;          // 回避ゲージのスケール値

public:
	// コンストラクタ
	CAvoidanceGauge();
	// デストラクタ
	~CAvoidanceGauge();

	// 回避ゲージを削除
	void Kill()override;

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