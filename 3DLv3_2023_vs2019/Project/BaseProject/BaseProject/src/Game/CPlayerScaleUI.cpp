#include "CPlayerScaleUI.h"
#include "CText.h"

#define SHADOW_COUNT 8     // �e�̃J�E���g
#define SHADOW_WIDTH 2.0f  // �e�̕�

// �R���X�g���N
CPlayerScaleUI::CPlayerScaleUI(float x, float y)
	: mBasePos(x + SHADOW_WIDTH, y)
{
	CVector2 size = CVector2(WINDOW_WIDTH, WINDOW_HEIGHT);
	ETextAlignH textAlignH = ETextAlignH::eLeft;

	// �傫���̃e�L�X�g���쐬
	mpScaleText = new CText
	(
		nullptr, 24,
		mBasePos,
		size,
		CColor(1.0f, 1.0f, 1.0f),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpScaleText->SetTextAlignH(textAlignH);
	SetShow(false);
}

// �f�X�g���N�^
CPlayerScaleUI::~CPlayerScaleUI()
{
	SAFE_DELETE(mpScaleText);
}

// �傫����ݒ�
void CPlayerScaleUI::SetScale(float scale)
{
	mpScaleText->SetText("%f", scale);
}

// �`��
void CPlayerScaleUI::Render()
{
	mpScaleText->SetColor(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < SHADOW_COUNT; i++)
	{
		CVector2 v = CVector2(1.0f, 0.0f);
		float alpha = (float)i / SHADOW_COUNT;
		CQuaternion rot = CQuaternion(0.0f, 0.0f, 360.0f * alpha);

		mpScaleText->SetPos(mBasePos + (CVector2)(rot * v) * SHADOW_WIDTH);
		mpScaleText->Render();
	}

	mpScaleText->SetColor(1.0f, 1.0f, 1.0f);
	mpScaleText->SetPos(mBasePos);
	mpScaleText->Render();
}