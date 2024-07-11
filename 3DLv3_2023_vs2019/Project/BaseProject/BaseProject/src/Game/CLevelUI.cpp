#include "CLevelUI.h"
#include "CText.h"

#define SHADOW_COUNT 8     // 影のカウント
#define SHADOW_WIDTH 2.0f  // 影の幅

// コンストラク
CLevelUI::CLevelUI()
	: mBasePos(15.0f + SHADOW_WIDTH, 32.0f)
{
	CVector2 size = CVector2(WINDOW_WIDTH, WINDOW_HEIGHT);
	ETextAlignH textAlignH = ETextAlignH::eLeft;

	// レベルのテキストを作成
	mpLevelText = new CText
	(
		nullptr, 24,
		mBasePos,
		size,
		CColor(1.0f, 1.0f, 1.0f),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpLevelText->SetTextAlignH(textAlignH);
}

// デストラクタ
CLevelUI::~CLevelUI()
{
	SAFE_DELETE(mpLevelText);
}

// Levelを設定
void CLevelUI::SetLevel(std::string level)
{
	mpLevelText->SetText(level.c_str());
}

// 更新
void CLevelUI::Update()
{
}

// 描画
void CLevelUI::Render()
{
	mpLevelText->SetColor(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < SHADOW_COUNT; i++)
	{
		CVector2 v = CVector2(1.0f, 0.0f);
		float alpha = (float)i / SHADOW_COUNT;
		CQuaternion rot = CQuaternion(0.0f, 0.0f, 360.0f * alpha);
		mpLevelText->SetPos(mBasePos + (CVector2)(rot * v) * SHADOW_WIDTH);
		mpLevelText->Render();
	}
	mpLevelText->SetColor(1.0f, 1.0f, 1.0f);
	mpLevelText->SetPos(mBasePos);
	mpLevelText->Render();
}