#include "CPlayerMaxHpUI.h"
#include "CText.h"

#define SHADOW_COUNT 8     // 影のカウント
#define SHADOW_WIDTH 2.0f  // 影の幅

// コンストラク
CPlayerMaxHpUI::CPlayerMaxHpUI(float x, float y)
	: mBasePos(x + SHADOW_WIDTH, y)
{
	CVector2 size = CVector2(WINDOW_WIDTH, WINDOW_HEIGHT);
	ETextAlignH textAlignH = ETextAlignH::eLeft;

	// 最大HPのテキストを作成
	mpMaxHpText = new CText
	(
		nullptr, 24,
		mBasePos,
		size,
		CColor(1.0f, 1.0f, 1.0f),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpMaxHpText->SetTextAlignH(textAlignH);
	SetShow(false);
}

// デストラクタ
CPlayerMaxHpUI::~CPlayerMaxHpUI()
{
	SAFE_DELETE(mpMaxHpText);
}

// 最大HPを設定
void CPlayerMaxHpUI::SetMaxHp(int maxHp)
{
	mpMaxHpText->SetText("%d", maxHp);
}

// 描画
void CPlayerMaxHpUI::Render()
{
	mpMaxHpText->SetColor(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < SHADOW_COUNT; i++)
	{
		CVector2 v = CVector2(1.0f, 0.0f);
		float alpha = (float)i / SHADOW_COUNT;
		CQuaternion rot = CQuaternion(0.0f, 0.0f, 360.0f * alpha);

		mpMaxHpText->SetPos(mBasePos + (CVector2)(rot * v) * SHADOW_WIDTH);
		mpMaxHpText->Render();
	}

	mpMaxHpText->SetColor(1.0f, 1.0f, 1.0f);
	mpMaxHpText->SetPos(mBasePos);
	mpMaxHpText->Render();
}