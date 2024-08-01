#include "CLevelUpUI.h"
#include "CCharaBase.h"

// �R���X�g���N�^
CLevelUpUI::CLevelUpUI(float height)
	: CBillBoardImage("UI/Text/level.png", ETag::eUI, ETaskPauseType::eGame)
	, mElapsedTime(0.0f)
	, mBaseScale(1.0f)
	, mHeight(height)
{
	SetEnable(false);
	SetShow(false);
}

// �f�X�g���N�^
CLevelUpUI::~CLevelUpUI()
{
	Kill();
}

// ������̐ݒ�
void CLevelUpUI::SetOwner(CCharaBase* owner)
{
	CBillBoardImage::SetOwner(owner);
	mBaseScale = owner->Scale().X();
}

// �J�n
void CLevelUpUI::StartLevelUpUI()
{
	SetEnable(true);
	SetShow(true);
}

// �I��
void CLevelUpUI::EndLevelUpUI()
{
	SetEnable(false);
	SetShow(false);
}

// ���g�̃x�[�X�X�P�[���l���Z�o
float CLevelUpUI::CalcScale() const
{
	return mOwner->Scale().X() / mBaseScale;
}

// �X�V
void CLevelUpUI::Update()
{
	// ���N���X�̍X�V����
	CBillBoardImage::Update();

	mElapsedTime += Time::DeltaTime();

	float scale = CalcScale();
	Scale(scale * 5.0f, scale * 5.0f, scale * 5.0f);
	CVector center = mOwner->Position();
	CVector pos = CVector::zero;
	pos.Y(mHeight);
	Position(center + pos * scale);

	if (mElapsedTime >= 2.0f)
	{
		mElapsedTime = 0.0f;
		EndLevelUpUI();
	}
}