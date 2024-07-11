#include "CHpUI.h"
#include "CText.h"

#define SHADOW_COUNT 8     // 影のカウント
#define SHADOW_WIDTH 2.0f  // 影の幅

// コンストラク
CHpUI::CHpUI()
	: mBasePos(460.0f + SHADOW_WIDTH, 82.0f)
{
	CVector2 size = CVector2(WINDOW_WIDTH, WINDOW_HEIGHT);
	ETextAlignH textAlignH = ETextAlignH::eLeft;

	// Hpのテキストを作成
	mpHpText = new CText
	(
		nullptr, 24,
		mBasePos,
		size,
		CColor(0.0f, 1.0f, 0.0f),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpHpText->SetTextAlignH(textAlignH);
	mpHpText->SetFontSize(20);
}

// デストラクタ
CHpUI::~CHpUI()
{
	SAFE_DELETE(mpHpText);
}

// Hpを設定
void CHpUI::SetHpText(std::string hpText)
{
	mpHpText->SetText(hpText.c_str());
}

// 描画
void CHpUI::Render()
{
	mpHpText->SetColor(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < SHADOW_COUNT; i++)
	{
		CVector2 v = CVector2(1.0f, 0.0f);
		float alpha = (float)i / SHADOW_COUNT;
		CQuaternion rot = CQuaternion(0.0f, 0.0f, 360.0f * alpha);
		mpHpText->SetPos(mBasePos + (CVector2)(rot * v) * SHADOW_WIDTH);
		mpHpText->Render();
	}
	mpHpText->SetColor(0.0f, 1.0f, 0.0f);
	mpHpText->SetPos(mBasePos);
	mpHpText->Render();
}