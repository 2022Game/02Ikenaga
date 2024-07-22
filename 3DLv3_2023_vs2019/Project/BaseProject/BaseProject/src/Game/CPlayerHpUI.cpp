#include "CPlayerHpUI.h"
#include "CText.h"

#define SHADOW_COUNT 8     // 影のカウント
#define SHADOW_WIDTH 2.0f  // 影の幅

// コンストラク
CPlayerHpUI::CPlayerHpUI(float x, float y)
	: mBasePos(x + SHADOW_WIDTH, y)
{
	CVector2 size = CVector2(WINDOW_WIDTH, WINDOW_HEIGHT);
	ETextAlignH textAlignH = ETextAlignH::eLeft;

	// HPのテキストを作成
	mpHpText = new CText
	(
		nullptr, 24,
		mBasePos,
		size,
		CColor(1.0f, 1.0f, 1.0f),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpHpText->SetTextAlignH(textAlignH);
	SetShow(false);
}

// デストラクタ
CPlayerHpUI::~CPlayerHpUI()
{
	SAFE_DELETE(mpHpText);
}

// HPを設定
void CPlayerHpUI::SetHp(int hp)
{
	mpHpText->SetText("%d", hp);
}

// 描画
void CPlayerHpUI::Render()
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

	mpHpText->SetColor(1.0f, 1.0f, 1.0f);
	mpHpText->SetPos(mBasePos);
	mpHpText->Render();
}