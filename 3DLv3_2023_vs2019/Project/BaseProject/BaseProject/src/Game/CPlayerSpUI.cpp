#include "CPlayerSpUI.h"
#include "CText.h"

#define SHADOW_COUNT 8     // 影のカウント
#define SHADOW_WIDTH 2.0f  // 影の幅

// コンストラク
CPlayerSpUI::CPlayerSpUI(float x, float y)
	: mBasePos(x + SHADOW_WIDTH, y)
{
	CVector2 size = CVector2(WINDOW_WIDTH, WINDOW_HEIGHT);
	ETextAlignH textAlignH = ETextAlignH::eLeft;

	// SPのテキストを作成
	mpSpText = new CText
	(
		nullptr, 24,
		mBasePos,
		size,
		CColor(1.0f, 1.0f, 1.0f),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpSpText->SetTextAlignH(textAlignH);
	SetShow(false);
}

// デストラクタ
CPlayerSpUI::~CPlayerSpUI()
{
	SAFE_DELETE(mpSpText);
}

// SPを設定
void CPlayerSpUI::SetSp(int sp)
{
	mpSpText->SetText("%d", sp);
}

// 描画
void CPlayerSpUI::Render()
{
	mpSpText->SetColor(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < SHADOW_COUNT; i++)
	{
		CVector2 v = CVector2(1.0f, 0.0f);
		float alpha = (float)i / SHADOW_COUNT;
		CQuaternion rot = CQuaternion(0.0f, 0.0f, 360.0f * alpha);

		mpSpText->SetPos(mBasePos + (CVector2)(rot * v) * SHADOW_WIDTH);
		mpSpText->Render();
	}

	mpSpText->SetColor(1.0f, 1.0f, 1.0f);
	mpSpText->SetPos(mBasePos);
	mpSpText->Render();
}