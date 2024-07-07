#include "CSpGauge.h"
#include "CImage.h"
#include "Maths.h"

// �t���[���̉��̃T�C�Y
#define FRAME_SIZE_X (250.0f)
// �t���[���̏c�̃T�C�Y
#define FRAME_SIZE_Y (30.0f)
// �s���N�̕�
#define FRAME_BORDER (2.0f)
// �o�[�̉��̃T�C�Y
#define BAR_SIZE_X (FRAME_SIZE_X - FRAME_BORDER*2.0f)
// �o�[�̏c�̃T�C�Y
#define BAR_SIZE_Y (FRAME_SIZE_Y - FRAME_BORDER*2.0f)


// �R���X�g���N�^
CSpGauge::CSpGauge()
	: mMaxValue(100)
	, mValue(100)
{
	mpFrameImage = new CImage("Frame");
	mpFrameImage->SetSize(FRAME_SIZE_X, FRAME_SIZE_Y);

	mpBarImage = new CImage("Gauge");
	mpBarImage->SetSize(BAR_SIZE_X, BAR_SIZE_Y);

	mpEdgeImage = new CImage("FrameEdge");
	mpEdgeImage->SetSize(FRAME_SIZE_X, FRAME_SIZE_Y);
}

// �f�X�g���N�^
CSpGauge::~CSpGauge()
{
	CTask::Kill();
	mpFrameImage->Kill();
	mpBarImage->Kill();
	mpEdgeImage->Kill();
}

// �ő�l��ݒ�
void CSpGauge::SetMaxValue(int value)
{
	mMaxValue = value;
}

// ���ݒl
void CSpGauge::SetValue(int value)
{
	mValue = value;
}

// �X�V
void CSpGauge::Update()
{
	// �Q�[�W�̃t���[���ƃo�[�Ɖ��̈ʒu��ݒ�
	mpFrameImage->SetPos(mPosition);
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
	if (percent <= 1.0f) color = CColor(1.0f, 0.0f, 0.8f);
	// �o�[�ɐF��ݒ�
	mpBarImage->SetColor(color);
}