#pragma once
#include "CUIBase.h"

class CText;

// ExpのUI
class CExpUI : public CUIBase
{
public:

	// コンストラク
	CExpUI();

	// デストラクタ
	~CExpUI();

	// Expを設定
	void SetExpText(std::string expText);

	// 描画
	void Render()override;
private:
	CText* mpExpText;   // Expテキスト
	CVector2 mBasePos;  // ベースの位置
};