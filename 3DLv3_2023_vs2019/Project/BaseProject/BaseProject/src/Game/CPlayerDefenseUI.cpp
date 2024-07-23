#include "CPlayerDefenseUI.h"
#include "CText.h"

#define SHADOW_COUNT 8     // 影のカウント
#define SHADOW_WIDTH 2.0f  // 影の幅

// コンストラク
CPlayerDefenseUI::CPlayerDefenseUI(float x, float y)
	: mBasePos(x + SHADOW_WIDTH, y)
{
	CVector2 size = CVector2(WINDOW_WIDTH, WINDOW_HEIGHT);
	ETextAlignH textAlignH = ETextAlignH::eLeft;

	// 防御力のテキストを作成
	mpDefenseText = new CText
	(
		nullptr, 24,
		mBasePos,
		size,
		CColor(1.0f, 1.0f, 1.0f),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpDefenseText->SetTextAlignH(textAlignH);
	SetShow(false);
}

// デストラクタ
CPlayerDefenseUI::~CPlayerDefenseUI()
{
	SAFE_DELETE(mpDefenseText);
}

// 防御力を設定
void CPlayerDefenseUI::SetDefense(int defense)
{
	mpDefenseText->SetText("%d", defense);
}

// 描画
void CPlayerDefenseUI::Render()
{
	mpDefenseText->SetColor(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < SHADOW_COUNT; i++)
	{
		CVector2 v = CVector2(1.0f, 0.0f);
		float alpha = (float)i / SHADOW_COUNT;
		CQuaternion rot = CQuaternion(0.0f, 0.0f, 360.0f * alpha);

		mpDefenseText->SetPos(mBasePos + (CVector2)(rot * v) * SHADOW_WIDTH);
		mpDefenseText->Render();
	}

	mpDefenseText->SetColor(1.0f, 1.0f, 1.0f);
	mpDefenseText->SetPos(mBasePos);
	mpDefenseText->Render();
}