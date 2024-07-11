#include "CSpUI.h"
#include "CText.h"

#define SHADOW_COUNT 8     // 影のカウント
#define SHADOW_WIDTH 2.0f  // 影の幅

// コンストラク
CSpUI::CSpUI()
	: mBasePos(260.0f + SHADOW_WIDTH, 112.0f)
{
	CVector2 size = CVector2(WINDOW_WIDTH, WINDOW_HEIGHT);
	ETextAlignH textAlignH = ETextAlignH::eLeft;

	// Spのテキストを作成
	mpSpText = new CText
	(
		nullptr, 24,
		mBasePos,
		size,
		CColor(1.0f, 0.0f, 0.8f),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpSpText->SetTextAlignH(textAlignH);
	mpSpText->SetFontSize(20);
}

// デストラクタ
CSpUI::~CSpUI()
{
	SAFE_DELETE(mpSpText);
}

// Spを設定
void CSpUI::SetSpText(std::string spText)
{
	mpSpText->SetText(spText.c_str());
}

// 描画
void CSpUI::Render()
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
	mpSpText->SetColor(1.0f, 0.0f, 0.8f);
	mpSpText->SetPos(mBasePos);
	mpSpText->Render();
}