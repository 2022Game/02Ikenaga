#pragma once
#include "CUIBase.h"

class CText;

// ロックレベル2
class CLockLevel2 : public CUIBase
{
public:
	// コンストラク
	CLockLevel2(float x, float y);

	// デストラクタ
	~CLockLevel2();

	// Levelを設定
	void SetLevel2(std::string level);

	// 更新
	void Update();

	// 描画
	void Render();

private:
	CText* mpLockLevel;
	int mLevel;
	CVector2 mBasePos;  // ベース位置
};