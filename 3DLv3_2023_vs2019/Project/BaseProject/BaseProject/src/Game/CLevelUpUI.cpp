#include "CLevelUpUI.h"
#include "CCharaBase.h"

// コンストラクタ
CLevelUpUI::CLevelUpUI(float height)
	: CBillBoardImage("UI/Text/level.png", ETag::eUI, ETaskPauseType::eGame)
	, mElapsedTime(0.0f)
	, mBaseScale(1.0f)
	, mHeight(height)
{
	SetEnable(false);
	SetShow(false);
}

// デストラクタ
CLevelUpUI::~CLevelUpUI()
{
	Kill();
}

// 持ち主の設定
void CLevelUpUI::SetOwner(CCharaBase* owner)
{
	CBillBoardImage::SetOwner(owner);
	mBaseScale = owner->Scale().X();
}

// 開始
void CLevelUpUI::StartLevelUpUI()
{
	SetEnable(true);
	SetShow(true);
}

// 終了
void CLevelUpUI::EndLevelUpUI()
{
	SetEnable(false);
	SetShow(false);
}

// 自身のベーススケール値を算出
float CLevelUpUI::CalcScale() const
{
	return mOwner->Scale().X() / mBaseScale;
}

// 更新
void CLevelUpUI::Update()
{
	// 基底クラスの更新処理
	CBillBoardImage::Update();

	mElapsedTime += Time::DeltaTime();

	float scale = CalcScale();
	Scale(scale * 5.0f, scale * 5.0f, scale * 5.0f);
	CVector center = mOwner->Position();
	CVector pos = CVector::zero;
	pos.Y(mHeight);
	Position(center + pos * scale);

	if (mElapsedTime >= 2.0f)
	{
		mElapsedTime = 0.0f;
		EndLevelUpUI();
	}
}