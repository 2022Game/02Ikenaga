#pragma once
#include "CUIBase.h"

class CText;

// プレイヤー名のUI
class CPlayerNameUI : public CUIBase
{
public:

	// コンストラク
	CPlayerNameUI();

	// デストラクタ
	~CPlayerNameUI();

	// プレイヤー名を設定
	void SetPlayerName(std::string name);

	// 更新
	void Update()override;

	// 描画
	void Render()override;
private:
	CText* mpNameText;
	CVector2 mBasePos;
};