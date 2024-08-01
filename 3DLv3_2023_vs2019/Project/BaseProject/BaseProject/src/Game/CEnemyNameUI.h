#pragma once
#include "CUIBase.h"

class CText;

// エネミーの名前UI
class CEnemyNameUI : public CUIBase
{
public:

	// コンストラク
	CEnemyNameUI(bool is3dGauge);

	// デストラクタ
	~CEnemyNameUI();

	// エネミーの名前を設定
	void SetEnemyName(std::string name);

	// 中心位置のの割合の設定
	void SetCenterRatio(const CVector2& ratio);

	// ワールド座標を設定
	void SetWorldPos(const CVector& worldPos);

	// 描画
	void Render()override;

private:
	CText* mpNameText;     // 名前テキスト
	CVector2 mBasePos;     // ベース位置
	CVector2 mCenterRatio; // 中心位置の割合
	bool mIs3dGauge;       // 3D空間に配置するゲージかどうか
	float mMaxScale;
	float mMinScale;
};