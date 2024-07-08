#include "CAvoidanceGauge.h"
#include "CImage.h"
#include "Maths.h"
#include "CCamera.h"
#include "CTransform.h"

// �t���[���̉��̃T�C�Y
#define FRAME_SIZE_X (20.0f)
// �t���[���̏c�̃T�C�Y
#define FRAME_SIZE_Y (150.0f)
// ���F�̕�
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
CAvoidanceGauge::CAvoidanceGauge(bool is3dGauge)
	: mMaxValue(100)
	, mValue(100)
	, mCenterRatio(0.0f, 0.0f)
	, mScale(1.0f)
	, mIs3dGauge(is3dGauge)
{
	mpFrameImage = new CImage("Frame");
	mpFrameImage->SetSize(FRAME_SIZE_X, FRAME_SIZE_Y);

	mpBarImage = new CImage("Gauge");
	mpBarImage->SetSize(BAR_SIZE_X, BAR_SIZE_Y);

	mpEdgeImage = new CImage("FrameEdge");
	mpEdgeImage->SetSize(FRAME_SIZE_X, FRAME_SIZE_Y);

	// �ŏ��͔�\��
	SetShow(false);
}

// �f�X�g���N�^
CAvoidanceGauge::~CAvoidanceGauge()
{
}

// ����Q�[�W���폜
void CAvoidanceGauge::Kill()
{
	CTask::Kill();
	mpFrameImage->Kill();
	mpBarImage->Kill();
	mpEdgeImage->Kill();
}

// �\�����邩�ǂ����ݒ�
void CAvoidanceGauge::SetShow(bool isShow)
{
	// �x�[�X�N���X�̕\���ݒ菈��
	CTask::SetShow(isShow);
	mpFrameImage->SetShow(isShow);
	mpBarImage->SetShow(isShow);
	mpEdgeImage->SetShow(isShow);
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
	// �Q�[�W��\�����Ȃ�
	if (screenPos.Z() < 0.0f)
	{
		SetShow(false);
		return;
	}

	// ���Ƃ߂��X�N���[�����W�����g�̈ʒu�ɐݒ�
	mPosition = screenPos;

	// �ݒ肳�ꂽ���[���h���W�ƃJ�����̍��W�����߂�
	float dist = (worldPos - cam->Position()).Length();

	if (dist <= SCALE_DIST_MAX)
	{
		// �J�������痣��邲�ƂɃX�P�[���l������������
		float ratio = 0.3f - Math::Clamp01((dist - SCALE_DIST_MIN) / (SCALE_DIST_MAX - SCALE_DIST_MIN));
		mScale = Math::Lerp(SCALE_MIN, SCALE_MAX, ratio);

		// �Q�[�W���\��
		SetShow(true);
	}
	// �J�����̋����������ꍇ�́A��\��
	else
	{
		SetShow(false);
	}
}

// �X�V
void CAvoidanceGauge::Update()
{
	// �Q�[�W�̃t���[���o�[�̈ʒu��ݒ�
	mpFrameImage->SetPos(mPosition);
	CVector2 barPos = mPosition;
	barPos.Y(barPos.Y() - FRAME_SIZE_Y * mCenterRatio.Y() * mScale);
	mpBarImage->SetPos(barPos + CVector2(FRAME_BORDER, FRAME_BORDER) * mScale);
	mpEdgeImage->SetPos(mPosition);

	// �t���[���T�C�Y��ύX
	mpFrameImage->SetSize(CVector2(FRAME_SIZE_X, -FRAME_SIZE_Y) * mScale);
	mpEdgeImage->SetSize(CVector2(FRAME_SIZE_X, -FRAME_SIZE_Y) * mScale);

	// �o�[�̃T�C�Y���ő�l�ƌ��ݒl���狁�߂�
	float percent = Math::Clamp01((float)mValue / mMaxValue);
	CVector2 size = CVector2(BAR_SIZE_X , -BAR_SIZE_Y * percent) * mScale;
	mpBarImage->SetSize(size);

	// �t���[���̒��S�ʒu��ݒ�
	mpFrameImage->SetCenter
	(
		FRAME_SIZE_X * mCenterRatio.X() * mScale,
		FRAME_SIZE_Y * mCenterRatio.Y() * mScale
	);

	// �o�[�̒��S�ʒu��ݒ�
	mpBarImage->SetCenter
	(
		FRAME_SIZE_X * mCenterRatio.X() * mScale,
		1.5f
	);

	// �ӂ��̒��S�ʒu��ݒ�
	mpEdgeImage->SetCenter
	(
		FRAME_SIZE_X * mCenterRatio.X() * mScale,
		FRAME_SIZE_Y * mCenterRatio.Y() * mScale
	);

	CColor color;
	// ���F
	if (percent <= 1.0f) color = CColor::white;
	// �o�[�ɐF��ݒ�
	mpBarImage->SetColor(color);
}