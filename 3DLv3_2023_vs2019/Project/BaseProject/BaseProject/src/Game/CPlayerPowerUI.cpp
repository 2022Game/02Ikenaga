#include "CPlayerPowerUI.h"
#include "CText.h"

#define SHADOW_COUNT 8     // 影のカウント
#define SHADOW_WIDTH 2.0f  // 影の幅

// コンストラク
CPlayerPowerUI::CPlayerPowerUI(float x, float y)
	: mBasePos(x + SHADOW_WIDTH, y)
{
	CVector2 size = CVector2(WINDOW_WIDTH, WINDOW_HEIGHT);
	ETextAlignH textAlignH = ETextAlignH::eLeft;

	// 攻撃力のテキストを作成
	mpPowerText = new CText
	(
		nullptr, 24,
		mBasePos,
		size,
		CColor(1.0f, 1.0f, 1.0f),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpPowerText->SetTextAlignH(textAlignH);
	SetShow(false);
}

// デストラクタ
CPlayerPowerUI::~CPlayerPowerUI()
{
	SAFE_DELETE(mpPowerText);
}

// 攻撃力を設定
void CPlayerPowerUI::SetPower(int power)
{
	mpPowerText->SetText("%d", power);
}

// 描画
void CPlayerPowerUI::Render()
{
	mpPowerText->SetColor(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < SHADOW_COUNT; i++)
	{
		CVector2 v = CVector2(1.0f, 0.0f);
		float alpha = (float)i / SHADOW_COUNT;
		CQuaternion rot = CQuaternion(0.0f, 0.0f, 360.0f * alpha);

		mpPowerText->SetPos(mBasePos + (CVector2)(rot * v) * SHADOW_WIDTH);
		mpPowerText->Render();
	}

	mpPowerText->SetColor(1.0f, 1.0f, 1.0f);
	mpPowerText->SetPos(mBasePos);
	mpPowerText->Render();
}