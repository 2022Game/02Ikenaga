#include "CPlayerNameUI.h"
#include "CText.h"

#define SHADOW_COUNT 8
#define SHADOW_WIDTH 2.0f

// �R���X�g���N
CPlayerNameUI::CPlayerNameUI()
	: mBasePos(0.0f + SHADOW_WIDTH, 30.0f)
{
	CVector2 size = CVector2(WINDOW_WIDTH, WINDOW_HEIGHT);
	ETextAlignH textAlignH = ETextAlignH::eLeft;

	// ���O�̃e�L�X�g���쐬
	mpNameText = new CText
	(
		nullptr,24,
		mBasePos,
		size,
		CColor(1.0f,1.0f,1.0f),
		ETaskPriority::eUI,0,
		ETaskPauseType::eGame,
		false,false
	);
	mpNameText->SetTextAlignH(textAlignH);
}

// �f�X�g���N�^
CPlayerNameUI::~CPlayerNameUI()
{
	SAFE_DELETE(mpNameText);
}

// �v���C���[����ݒ�
void CPlayerNameUI::SetPlayerName(std::string name)
{
	mpNameText->SetText(name.c_str());
}

// �X�V
void CPlayerNameUI::Update()
{
}

// �`��
void CPlayerNameUI::Render()
{
	mpNameText->SetColor(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < SHADOW_COUNT; i++)
	{
		CVector2 v = CVector2(1.0f, 0.0f);
		float alpha = (float)i / SHADOW_COUNT;
		CQuaternion rot = CQuaternion(0.0f, 0.0f, 360.0f * alpha);
		mpNameText->SetPos(mBasePos + (CVector2)(rot * v) * SHADOW_WIDTH);
		mpNameText->Render();
	}
	mpNameText->SetColor(1.0f, 1.0f, 1.0f);
	mpNameText->SetPos(mBasePos);
	mpNameText->Render();
}
