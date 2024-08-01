#include "CBossUI.h"
#include "CCharaBase.h"
#include "Maths.h"
#include "Easing.h"

// �R���X�g���N�^
CBossUI::CBossUI(float height)
	: CBillBoardImage("UI/Text/boss.png", ETag::eUI, ETaskPauseType::eGame)
	, mBaseScale(1.0f)
	, mHeight(height)
{
	SetEnable(false);
	SetShow(false);
}

// �f�X�g���N�^
CBossUI::~CBossUI()
{
}

// ������̐ݒ�
void CBossUI::SetOwner(CCharaBase* owner)
{
	CBillBoardImage::SetOwner(owner);
}

// �X�V
void CBossUI::Update()
{
	// ���N���X�̍X�V����
	CBillBoardImage::Update();

	CVector center = mOwner->Position();
	CVector pos = CVector::zero;
	pos.Y(mHeight);
	Position(center + pos);
}