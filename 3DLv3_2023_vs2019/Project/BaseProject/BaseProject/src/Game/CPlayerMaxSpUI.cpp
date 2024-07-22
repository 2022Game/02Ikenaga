#include "CPlayerMaxSpUI.h"
#include "CText.h"

#define SHADOW_COUNT 8     // �e�̃J�E���g
#define SHADOW_WIDTH 2.0f  // �e�̕�

// �R���X�g���N
CPlayerMaxSpUI::CPlayerMaxSpUI(float x, float y)
	: mBasePos(x + SHADOW_WIDTH, y)
{
	CVector2 size = CVector2(WINDOW_WIDTH, WINDOW_HEIGHT);
	ETextAlignH textAlignH = ETextAlignH::eLeft;

	// �ő�SP�̃e�L�X�g���쐬
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

// �f�X�g���N�^
CPlayerMaxSpUI::~CPlayerMaxSpUI()
{
	SAFE_DELETE(mpMaxSpText);
}

// �ő�SP��ݒ�
void CPlayerMaxSpUI::SetMaxSp(int maxSp)
{
	mpMaxSpText->SetText("%d", maxSp);
}

// �`��
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