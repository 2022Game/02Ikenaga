#include "CPlayerLevelUI.h"
#include "CText.h"

#define SHADOW_COUNT 8     // �e�̃J�E���g
#define SHADOW_WIDTH 2.0f  // �e�̕�

// �R���X�g���N
CPlayerLevelUI::CPlayerLevelUI(float x, float y)
	: mBasePos(x + SHADOW_WIDTH, y)
{
	CVector2 size = CVector2(WINDOW_WIDTH, WINDOW_HEIGHT);
	ETextAlignH textAlignH = ETextAlignH::eLeft;

	// ���x���̃e�L�X�g���쐬
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
	SetShow(false);
}

// �f�X�g���N�^
CPlayerLevelUI::~CPlayerLevelUI()
{
	SAFE_DELETE(mpLevelText);
}

// �v���C���[���x����ݒ�
void CPlayerLevelUI::SetLevel(int level)
{
	mpLevelText->SetText("%d",level);
}

// �`��
void CPlayerLevelUI::Render()
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