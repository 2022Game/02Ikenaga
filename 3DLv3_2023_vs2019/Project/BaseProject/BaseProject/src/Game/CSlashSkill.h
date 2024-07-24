#pragma once
#include "CUIBase.h"

class CImage;

// スラッシュスキル
class CSlashSkill : public CUIBase
{
public:
	// コンストラク
	CSlashSkill(float x, float y);

	// デストラクタ
	~CSlashSkill();

private:
	CImage* mpSlashSkill;
};