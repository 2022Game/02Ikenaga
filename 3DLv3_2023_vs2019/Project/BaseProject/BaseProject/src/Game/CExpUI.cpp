#include "CExpUI.h"
#include "CText.h"

#define SHADOW_COUNT 8     // �e�̃J�E���g
#define SHADOW_WIDTH 2.0f  // �e�̕�

// �R���X�g���N
CExpUI::CExpUI()
	: mBasePos(293.0f + SHADOW_WIDTH, 701.0f)
{
	CVector2 size = CVector2(WINDOW_WIDTH, WINDOW_HEIGHT);
	ETextAlignH textAlignH = ETextAlignH::eLeft;

	// Exp�̃e�L�X�g���쐬
	mpExpText = new CText
	(
		nullptr, 24,
		mBasePos,
		size,
		CColor(1.0f, 1.0f, 0.5f),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpExpText->SetTextAlignH(textAlignH);
	mpExpText->SetFontSize(15);
}

// �f�X�g���N�^
CExpUI::~CExpUI()
{
	SAFE_DELETE(mpExpText);
}

// Exp��ݒ�
void CExpUI::SetExpText(std::string expText)
{
	mpExpText->SetText(expText.c_str());
}

// �`��
void CExpUI::Render()
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
	mpExpText->SetColor(1.0f, 1.0f, 0.5f);
	mpExpText->SetPos(mBasePos);
	mpExpText->Render();
}