#include "CBossUI.h"
#include "CCharaBase.h"
#include "Maths.h"
#include "Easing.h"

// コンストラクタ
CBossUI::CBossUI(float height)
	: CBillBoardImage("UI/Text/boss.png", ETag::eUI, ETaskPauseType::eGame)
	, mBaseScale(1.0f)
	, mHeight(height)
{
	SetEnable(false);
	SetShow(false);
}

// デストラクタ
CBossUI::~CBossUI()
{
}

// 持ち主の設定
void CBossUI::SetOwner(CCharaBase* owner)
{
	CBillBoardImage::SetOwner(owner);
}

// 更新
void CBossUI::Update()
{
	// 基底クラスの更新処理
	CBillBoardImage::Update();

	CVector center = mOwner->Position();
	CVector pos = CVector::zero;
	pos.Y(mHeight);
	Position(center + pos);
}