#include "CLockLevel2.h"
#include "CText.h"
#include "CPlayer.h"

#define SHADOW_COUNT 8     // 影のカウント
#define SHADOW_WIDTH 2.0f  // 影の幅

// コンストラク
CLockLevel2::CLockLevel2(float x, float y)
	: mLevel(0)
	, mBasePos(x + SHADOW_WIDTH, y)
{
	CVector2 size = CVector2(WINDOW_WIDTH * 0.5, WINDOW_HEIGHT * 0.5f);
	ETextAlignH textAlignH = ETextAlignH::eLeft;

	// レベルのテキストを作成
	mpLockLevel = new CText
	(
		nullptr, 24,
		mBasePos,
		size,
		CColor(1.0f, 1.0f, 1.0f),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpLockLevel->SetTextAlignH(textAlignH);
	mpLockLevel->SetFontSize(20);
}

// デストラクタ
CLockLevel2::~CLockLevel2()
{
	SAFE_DELETE(mpLockLevel)
}

// Levelを設定
void CLockLevel2::SetLevel2(std::string level)
{
	mpLockLevel->SetText(level.c_str());
}

// 更新
void CLockLevel2::Update()
{
	CPlayer* player = CPlayer::Instance();
	mLevel = player->Status().level;
	if (mLevel >= 10)
	{
		Kill();
	}
}

// 描画
void CLockLevel2::Render()
{
	mpLockLevel->SetColor(1.0f, 1.0f, 1.0f);

	for (int i = 0; i < SHADOW_COUNT; i++)
	{
		CVector2 v = CVector2(1.0f, 0.0f);
		float alpha = (float)i / SHADOW_COUNT;
		CQuaternion rot = CQuaternion(0.0f, 0.0f, 360.0f * alpha);

		// レベル
		mpLockLevel->SetPos(mBasePos + (CVector2)(rot * v) * SHADOW_WIDTH);
		mpLockLevel->Render();
	}
	mpLockLevel->SetColor(0.0f, 0.0f, 0.0f);
	mpLockLevel->SetPos(mBasePos);
	mpLockLevel->Render();
}