#include "CSaGauge.h"
#include "CImage.h"
#include "Maths.h"

// �Q�[�W�̃t���[���摜�̃t�@�C���p�X
#define FRAME_IMAGE "Character\\Player\\HP\\Frame.png"  //HP�t���[���摜
// �Q�[�W�̃o�[�摜�̃t�@�C���p�X
#define BAR_IMAGE "UI\\white.png"
// �Q�[�W�̂ӂ�
#define EDGE_IMAGE "Character\\Player\\HP\\FrameEdge.png"

// �t���[���̉��̃T�C�Y
#define FRAME_SIZE_X (250.0f)
// �t���[���̏c�̃T�C�Y
#define FRAME_SIZE_Y (30.0f)
// �΂̕�
#define FRAME_BORDER (2.0f)
// �o�[�̉��̃T�C�Y
#define BAR_SIZE_X (FRAME_SIZE_X - FRAME_BORDER*2.0f)
// �o�[�̏c�̃T�C�Y
#define BAR_SIZE_Y (FRAME_SIZE_Y - FRAME_BORDER*2.0f)


// �R���X�g���N�^
CSaGauge::CSaGauge()
	:mMaxValue(100)
	, mValue(100)
{
	mpFrameImage = new CImage(FRAME_IMAGE);
	mpFrameImage->SetSize(FRAME_SIZE_X, FRAME_SIZE_Y);

	mpBarImage = new CImage(BAR_IMAGE);
	mpBarImage->SetSize(BAR_SIZE_X, BAR_SIZE_Y);

	mpEdgeImage = new CImage(EDGE_IMAGE);
	mpEdgeImage->SetSize(FRAME_SIZE_X, FRAME_SIZE_Y);
}

// �f�X�g���N�^
CSaGauge::~CSaGauge()
{
	CTask::Kill();
	mpFrameImage->Kill();
	mpBarImage->Kill();
	mpEdgeImage->Kill();
}

// �ő�l��ݒ�
void CSaGauge::SetMaxValue(int value)
{
	mMaxValue = value;
}

// ���ݒl
void CSaGauge::SetValue(int value)
{
	mValue = value;
}

// �X�V
void CSaGauge::Update()
{
	// �Q�[�W�̃t���[�������o�[�̈ʒu��ݒ�
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