#include "CPlayerMaxSpUI.h"
#include "CText.h"

#define SHADOW_COUNT 8     // 影のカウント
#define SHADOW_WIDTH 2.0f  // 影の幅

// コンストラク
CPlayerMaxSpUI::CPlayerMaxSpUI(float x, float y)
	: mBasePos(x + SHADOW_WIDTH, y)
{
	CVector2 size = CVector2(WINDOW_WIDTH, WINDOW_HEIGHT);
	ETextAlignH textAlignH = ETextAlignH::eLeft;

	// 最大SPのテキストを作成
	mpMaxSpText = new CText
	(
		nullptr, 24,
		mBasePos,
		size,
		CColor(1.0f, 1.0f, 1.0f),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpMaxSpText->SetTextAlignH(textAlignH);
	SetShow(false);
}

// デストラクタ
CPlayerMaxSpUI::~CPlayerMaxSpUI()
{
	SAFE_DELETE(mpMaxSpText);
}

// 最大SPを設定
void CPlayerMaxSpUI::SetMaxSp(int maxSp)
{
	mpMaxSpText->SetText("%d", maxSp);
}

// 描画
void CPlayerMaxSpUI::Render()
{
	mpMaxSpText->SetColor(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < SHADOW_COUNT; i++)
	{
		CVector2 v = CVector2(1.0f, 0.0f);
		float alpha = (float)i / SHADOW_COUNT;
		CQuaternion rot = CQuaternion(0.0f, 0.0f, 360.0f * alpha);

		mpMaxSpText->SetPos(mBasePos + (CVector2)(rot * v) * SHADOW_WIDTH);
		mpMaxSpText->Render();
	}

	mpMaxSpText->SetColor(1.0f, 1.0f, 1.0f);
	mpMaxSpText->SetPos(mBasePos);
	mpMaxSpText->Render();
}