#include "CDizzy.h"
#include "CCharaBase.h"
#include "Easing.h"

// �A�j���[�V������1�R�}�\������
#define ANIM_TIME 0.0825f

// �G�t�F�N�g�̃A�j���[�V�����f�[�^
TexAnimData CDizzy::msAnimData = TexAnimData(1, 5, true, 64, ANIM_TIME);

// �R���X�g���N�^
CDizzy::CDizzy()
	: CBillBoardImage("Effect/Dizzy.png", ETag::eDizzy,ETaskPauseType::eGame)
	, mElapsedTime(0.0f)
	, mIsDeath(false)
{
	SetAnimData(&msAnimData);
}

// �f�X�g���N�^
CDizzy::~CDizzy()
{
}

// �e�p�����[�^��ݒ�
void CDizzy::Setup(const CVector& pos)
{
	Position(pos);
}

// �폜�t���O�������Ă��邩�ǂ���
bool CDizzy::IsDeath() const
{
	return mIsDeath;
}

// �J���[��ݒ�
void CDizzy::SetColor(const CColor& color)
{
	mMaterial.Diffuse()[0] = color.R();
	mMaterial.Diffuse()[1] = color.G();
	mMaterial.Diffuse()[2] = color.B();
	mMaterial.Diffuse()[3] = color.A();
	CObjectBase::SetColor(color);
}

// �u�����h�^�C�v��ݒ�
void CDizzy::SetBlendType(EBlend type)
{
	mMaterial.SetBlendType(type);
}

// �X�V
void CDizzy::Update()
{
	// ���N���X�̍X�V����
	CBillBoardImage::Update();

	mElapsedTime += Time::DeltaTime();

	if (mElapsedTime >= 1.3)
	{
		Kill();
	}
}