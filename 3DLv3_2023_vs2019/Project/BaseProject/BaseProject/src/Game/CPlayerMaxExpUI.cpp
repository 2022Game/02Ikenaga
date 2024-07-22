#include "CPlayerMaxExpUI.h"
#include "CText.h"

#define SHADOW_COUNT 8     // �e�̃J�E���g
#define SHADOW_WIDTH 2.0f  // �e�̕�

// �R���X�g���N
CPlayerMaxExpUI::CPlayerMaxExpUI(float x, float y)
	: mBasePos(x + SHADOW_WIDTH, y)
{
	CVector2 size = CVector2(WINDOW_WIDTH, WINDOW_HEIGHT);
	ETextAlignH textAlignH = ETextAlignH::eLeft;

	// �ő�EXP�̃e�L�X�g���쐬
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

// �f�X�g���N�^
CPlayerMaxExpUI::~CPlayerMaxExpUI()
{
	SAFE_DELETE(mpMaxExpText);
}

// �ő�EXP��ݒ�
void CPlayerMaxExpUI::SetMaxExp(int maxExp)
{
	mpMaxExpText->SetText("%d", maxExp);
}

// �`��
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