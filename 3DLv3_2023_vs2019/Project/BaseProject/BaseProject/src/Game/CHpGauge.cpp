#include "CHpGauge.h"
#include "CImage.h"
#include "Maths.h"

//�Q�[�W�̃t���[���摜�̃t�@�C���p�X
#define FRAME_IMAGE "Character\\Player\\HP\\Frame.png"  //HP�t���[���摜
//�Q�[�W�̃o�[�摜�̃t�@�C���p�X
#define BAR_IMAGE "UI\\white.png"

//�t���[���̉��̃T�C�Y
#define FRAME_SIZE_X (250.0f)
//#define FRAME_SIZE_XX (430.0f)
//�t���[���̏c�̃T�C�Y
#define FRAME_SIZE_Y (30.0f)
//�΂̕�
#define FRAME_BORDER (2.0f)
//�o�[�̉��̃T�C�Y
#define BAR_SIZE_X (FRAME_SIZE_X - FRAME_BORDER*2.0f)
//�o�[�̏c�̃T�C�Y
#define BAR_SIZE_Y (FRAME_SIZE_Y - FRAME_BORDER*2.0f)


//�R���X�g���N�^
CHpGauge::CHpGauge()
	:mMaxValue(100)
	,mValue(100)
{
	mpFrameImage = new CImage(FRAME_IMAGE);
	mpFrameImage->SetSize(FRAME_SIZE_X, FRAME_SIZE_Y);

	mpBarImage = new CImage(BAR_IMAGE);
	mpBarImage->SetSize(BAR_SIZE_X, BAR_SIZE_Y);
}

//�f�X�g���N�^
CHpGauge::~CHpGauge()
{

}

//�ő�l��ݒ�
void CHpGauge::SetMaxValue(int value)
{
	mMaxValue = value;
}

//���ݒl
void CHpGauge::SetValue(int value)
{
	mValue = value;
}

//�X�V
void CHpGauge::Update()
{
	//�Q�[�W�̃t���[�������o�[�̈ʒu��ݒ�
	mpFrameImage->SetPos(mPosition);
	mpBarImage->SetPos(mPosition + CVector2(FRAME_BORDER, FRAME_BORDER));

	//�o�[�̃T�C�Y���ő�l�ƌ��ݒl���狁�߂�
	float percent = Math::Clamp01((float)mValue / mMaxValue);
	CVector2 size = CVector2(BAR_SIZE_X, BAR_SIZE_Y);
	size.X(BAR_SIZE_X * percent);
	mpBarImage->SetSize(size);

	//HP�̊����Ńo�[�̐F��ύX
	CColor color;
	//10%�ȉ��͐ԐF
	if (percent <= 0.1f) color = CColor(1.0f, 0.0f, 0.0f);
	//30%�ȉ��͉��F
	else if (percent <= 0.3f)color = CColor(1.0f, 1.0f, 0.0f);
	//����ȊO�͗ΐF
	else color = CColor(0.0f, 1.0f,0.0f);
	//�o�[�ɐF��ݒ�
	mpBarImage->SetColor(color);
}