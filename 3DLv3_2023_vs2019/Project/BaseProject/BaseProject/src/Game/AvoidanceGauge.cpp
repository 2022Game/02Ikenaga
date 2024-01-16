#include "CAvoidanceGauge.h"
#include "CImage.h"
#include "Maths.h"
#include "CCamera.h"

// �Q�[�W�̃t���[���摜�̃t�@�C���p�X
//#define FRAME_IMAGE "Character\\Player\\HP\\Frame.png"
//�Q�[�W�̃o�[�摜�̃t�@�C���p�X
//#define BAR_IMAGE "UI\\white.png"
// �Q�[�W�̂ӂ�
//#define EDGE_IMAGE "Character\\Player\\HP\\FrameEdge.png"

// �t���[���̉��̃T�C�Y
#define FRAME_SIZE_X (450.0f)
// �t���[���̏c�̃T�C�Y
#define FRAME_SIZE_Y (40.0f)
// �΂̕�
#define FRAME_BORDER (2.0f)
// �o�[�̉��̃T�C�Y
#define BAR_SIZE_X (FRAME_SIZE_X - FRAME_BORDER*2.0f)
// �o�[�̏c�̃T�C�Y
#define BAR_SIZE_Y (FRAME_SIZE_Y - FRAME_BORDER*2.0f)

// �X�P�[���l�v�Z���̃J�����Ƃ̋����̍ŏ�
#define SCALE_DIST_MIN 0.0f
// �X�P�[���l�v�Z���̃J�����Ƃ̋����̍ő�
#define SCALE_DIST_MAX 200.0f
// �X�P�[���l�̍ŏ��l
#define SCALE_MIN 0.7f
// �X�P�[���l�̍ő�l
#define SCALE_MAX 1.5f

// �R���X�g���N�^
CAvoidanceGauge::CAvoidanceGauge()
	:mMaxValue(100)
	, mValue(100)
	, mCenterRatio(0.0f, 0.0f)
	, mScale(1.0f)
{
	mpFrameImage = new CImage("AvoidanceGaugeFrame");
	mpFrameImage->SetSize(FRAME_SIZE_X, FRAME_SIZE_Y);

	mpBarImage = new CImage("AvoidanceGauge");
	mpBarImage->SetSize(BAR_SIZE_X, BAR_SIZE_Y);

	mpEdgeImage = new CImage("FrameEdge");
	mpEdgeImage->SetSize(FRAME_SIZE_X, FRAME_SIZE_Y);
}

// �f�X�g���N�^
CAvoidanceGauge::~CAvoidanceGauge()
{
}

// HP�Q�[�W���폜
void CAvoidanceGauge::Kill()
{
	CTask::Kill();
	mpFrameImage->Kill();
	mpBarImage->Kill();
	mpEdgeImage->Kill();
}

// �ő�l��ݒ�
void CAvoidanceGauge::SetMaxValue(int value)
{
	mMaxValue = value;
}

// ���ݒl
void CAvoidanceGauge::SetValue(int value)
{
	mValue = value;
}

// ���S�ʒu�̂̊����̐ݒ�
void CAvoidanceGauge::SetCenterRatio(const CVector2& ratio)
{
	mCenterRatio = ratio;
}

// ���[���h���W��ݒ�
void CAvoidanceGauge::SetWorldPos(const CVector& worldPos)
{
	// ���݂̃J�������擾
	CCamera* cam = CCamera::CurrentCamera();
	if (cam == nullptr)return;

	// �w�肳�ꂽ���[���h���W���X�N���[�����W�ɕϊ�
	CVector screenPos = cam->WorldToScreenPos(worldPos);

	// �ݒ胏�[���h���W���J�����̔w��ł���΁A
	// ����Q�[�W��\�����Ȃ�
	if (screenPos.Z() < 0.0f)
	{
		SetShow(false);
		return;
	}

	// ����Q�[�W��\��
	SetShow(true);
	// ���Ƃ߂��X�N���[�����W�����g�̈ʒu�ɐݒ�
	mPosition = screenPos;

	// �ݒ肳�ꂽ���[���h���W�ƃJ�����̍��W�����߂�
	float dist = (worldPos - cam->Position()).Length();

	// �J�������痣��邲�ƂɃX�P�[���l������������
	float ratio = 0.3f - Math::Clamp01((dist - SCALE_DIST_MIN) / (SCALE_DIST_MAX - SCALE_DIST_MIN));
	mScale = Math::Lerp(SCALE_MIN, SCALE_MAX, ratio);
}

// �X�V
void CAvoidanceGauge::Update()
{
	// �Q�[�W�̃t���[���o�[�̈ʒu��ݒ�
	mpFrameImage->SetPos(mPosition);
	CVector2 barPos = mPosition;
	barPos.X(barPos.X() - FRAME_SIZE_X * mCenterRatio.X() * mScale);
	mpBarImage->SetPos(barPos + CVector2(FRAME_BORDER, FRAME_BORDER) * mScale);
	mpEdgeImage->SetPos(mPosition);

	// �t���[���T�C�Y��ύX
	mpFrameImage->SetSize(CVector2(FRAME_SIZE_X, FRAME_SIZE_Y) * mScale);
	mpEdgeImage->SetSize(CVector2(FRAME_SIZE_X, FRAME_SIZE_Y) * mScale);

	// �o�[�̃T�C�Y���ő�l�ƌ��ݒl���狁�߂�
	float percent = Math::Clamp01((float)mValue / mMaxValue);
	CVector2 size = CVector2(BAR_SIZE_X * percent, BAR_SIZE_Y) * mScale;
	mpBarImage->SetSize(size);

	// �t���[���ƃo�[�̒��S�ʒu��ݒ�
	mpFrameImage->SetCenter
	(
		FRAME_SIZE_X * mCenterRatio.X() * mScale,
		FRAME_SIZE_Y * mCenterRatio.Y() * mScale
	);
	mpBarImage->SetCenter
	(
		0.0f,
		FRAME_SIZE_Y * mCenterRatio.Y() * mScale
	);
	mpEdgeImage->SetCenter
	(
		FRAME_SIZE_X * mCenterRatio.X() * mScale,
		FRAME_SIZE_Y * mCenterRatio.Y() * mScale
	);

	// ����̊����Ńo�[�̐F��ύX
	CColor color;
	// 10%�ȉ��͐ԐF
	if (percent <= 0.1f) color = CColor(1.0f, 0.0f, 0.0f);
	// 30%�ȉ��͉��F
	else if (percent <= 0.3f)color = CColor(1.0f, 1.0f, 0.0f);
	// ����ȊO�͗ΐF
	else color = CColor(0.0f, 1.0f, 0.0f);
	// �o�[�ɐF��ݒ�
	mpBarImage->SetColor(color);
}