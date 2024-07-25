#pragma once
#include "CUIBase.h"

class CText;

// ロックレベル
class CLockLevel : public CUIBase
{
public:
	// コンストラク
	CLockLevel(float x, float y);

	// デストラクタ
	~CLockLevel();

	// Levelを設定
	void SetLevel(std::string level);

	// 更新
	void Update();

	// 描画
	void Render();

private:
	CText* mpLockLevel;
	int mLevel;
	CVector2 mBasePos;  // ベース位置
};