#pragma once
#include "CUIBase.h"

class CText;

// LvのUI
class CLevelUI : public CUIBase
{
public:

	// コンストラク
	CLevelUI(bool is3dGauge);

	// デストラクタ
	~CLevelUI();

	// Levelを設定
	void SetLevel(std::string level);

	// 中心位置のの割合の設定
	void SetCenterRatio(const CVector2& ratio);

	// ワールド座標を設定
	void SetWorldPos(const CVector& worldPos);

	// 更新
	void Update()override;

	// 描画
	void Render()override;
private:
	CText* mpLevelText;    // レベルテキスト
	CVector2 mBasePos;     // ベース位置
	CVector2 mCenterRatio; // 中心位置の割合
	bool mIs3dGauge;       // 3D空間に配置するゲージかどうか
};