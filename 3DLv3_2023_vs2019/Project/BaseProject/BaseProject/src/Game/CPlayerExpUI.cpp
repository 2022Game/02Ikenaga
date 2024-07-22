#include "CPlayerExpUI.h"
#include "CText.h"

#define SHADOW_COUNT 8     // �e�̃J�E���g
#define SHADOW_WIDTH 2.0f  // �e�̕�

// �R���X�g���N
CPlayerExpUI::CPlayerExpUI(float x, float y)
	: mBasePos(x + SHADOW_WIDTH, y)
{
	CVector2 size = CVector2(WINDOW_WIDTH, WINDOW_HEIGHT);
	ETextAlignH textAlignH = ETextAlignH::eLeft;

	// EXP�̃e�L�X�g���쐬
	mpExpText = new CText
	(
		nullptr, 24,
		mBasePos,
		size,
		CColor(1.0f, 1.0f, 1.0f),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpExpText->SetTextAlignH(textAlignH);
	SetShow(false);
}

// �f�X�g���N�^
CPlayerExpUI::~CPlayerExpUI()
{
	SAFE_DELETE(mpExpText);
}

// EXP��ݒ�
void CPlayerExpUI::SetExp(int exp)
{
	mpExpText->SetText("%d", exp);
}

// �`��
void CPlayerExpUI::Render()
{
	mpExpText->SetColor(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < SHADOW_COUNT; i++)
	{
		CVector2 v = CVector2(1.0f, 0.0f);
		float alpha = (float)i / SHADOW_COUNT;
		CQuaternion rot = CQuaternion(0.0f, 0.0f, 360.0f * alpha);

		mpExpText->SetPos(mBasePos + (CVector2)(rot * v) * SHADOW_WIDTH);
		mpExpText->Render();
	}

	mpExpText->SetColor(1.0f, 1.0f, 1.0f);
	mpExpText->SetPos(mBasePos);
	mpExpText->Render();
}