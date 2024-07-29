#include "CExpGauge.h"
#include "CImage.h"
#include "Maths.h"

// �t���[���̉��̃T�C�Y
#define FRAME_SIZE_X (600.0f)
// �t���[���̏c�̃T�C�Y
#define FRAME_SIZE_Y (20.0f)
// �ΐF�̕�
#define FRAME_BORDER (2.0f)
// �o�[�̉��̃T�C�Y
#define BAR_SIZE_X (FRAME_SIZE_X - FRAME_BORDER*2.0f)
// �o�[�̏c�̃T�C�Y
#define BAR_SIZE_Y (FRAME_SIZE_Y - FRAME_BORDER*2.0f)


// �R���X�g���N�^
CExpGauge::CExpGauge()
	: mMaxValue(100)
	, mValue(100)
{
	mpBarImage = new CImage("Gauge");
	mpBarImage->SetSize(BAR_SIZE_X, BAR_SIZE_Y);

	mpEdgeImage = new CImage("FrameEdge");
	mpEdgeImage->SetSize(FRAME_SIZE_X, FRAME_SIZE_Y);
}

// �f�X�g���N�^
CExpGauge::~CExpGauge()
{
	CTask::Kill();
	mpBarImage->Kill();
	mpEdgeImage->Kill();
}

// �ő�l��ݒ�
void CExpGauge::SetMaxValue(int value)
{
	mMaxValue = value;
}

// ���ݒl
void CExpGauge::SetValue(int value)
{
	mValue = value;
}

// �\�����邩�ǂ����ݒ�
void CExpGauge::SetShow(bool isShow)
{
	// �x�[�X�N���X�̕\���ݒ菈��
	CTask::SetShow(isShow);
	mpBarImage->SetShow(isShow);
	mpEdgeImage->SetShow(isShow);
}

// �X�V
void CExpGauge::Update()
{
	// �Q�[�W�̃o�[�Ɖ��̈ʒu��ݒ�
	mpBarImage->SetPos(mPosition + CVector2(FRAME_BORDER, FRAME_BORDER));
	mpEdgeImage->SetPos(mPosition);

	// �o�[�̃T�C�Y���ő�l�ƌ��ݒl���狁�߂�
	float percent = Math::Clamp01((float)mValue / mMaxValue);
	CVector2 size = CVector2(BAR_SIZE_X, BAR_SIZE_Y);
	size.X(BAR_SIZE_X * percent);
	mpBarImage->SetSize(size);

	// SA�̊����Ńo�[�̐F��ύX
	CColor color;
	// �s���N�F
	if (percent <= 1.0f) color = CColor(0.0f, 0.5f, 0.0f, 0.8f);
	// �o�[�ɐF��ݒ�
	mpBarImage->SetColor(color);
}