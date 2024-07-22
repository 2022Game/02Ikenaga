#include "CPlayerMaxExpUI.h"
#include "CText.h"

#define SHADOW_COUNT 8     // 影のカウント
#define SHADOW_WIDTH 2.0f  // 影の幅

// コンストラク
CPlayerMaxExpUI::CPlayerMaxExpUI(float x, float y)
	: mBasePos(x + SHADOW_WIDTH, y)
{
	CVector2 size = CVector2(WINDOW_WIDTH, WINDOW_HEIGHT);
	ETextAlignH textAlignH = ETextAlignH::eLeft;

	// 最大EXPのテキストを作成
	mpMaxExpText = new CText
	(
		nullptr, 24,
		mBasePos,
		size,
		CColor(1.0f, 1.0f, 1.0f),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpMaxExpText->SetTextAlignH(textAlignH);
	SetShow(false);
}

// デストラクタ
CPlayerMaxExpUI::~CPlayerMaxExpUI()
{
	SAFE_DELETE(mpMaxExpText);
}

// 最大EXPを設定
void CPlayerMaxExpUI::SetMaxExp(int maxExp)
{
	mpMaxExpText->SetText("%d", maxExp);
}

// 描画
void CPlayerMaxExpUI::Render()
{
	mpMaxExpText->SetColor(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < SHADOW_COUNT; i++)
	{
		CVector2 v = CVector2(1.0f, 0.0f);
		float alpha = (float)i / SHADOW_COUNT;
		CQuaternion rot = CQuaternion(0.0f, 0.0f, 360.0f * alpha);

		mpMaxExpText->SetPos(mBasePos + (CVector2)(rot * v) * SHADOW_WIDTH);
		mpMaxExpText->Render();
	}

	mpMaxExpText->SetColor(1.0f, 1.0f, 1.0f);
	mpMaxExpText->SetPos(mBasePos);
	mpMaxExpText->Render();
}