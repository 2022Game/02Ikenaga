#include "CHpGauge.h"
#include "CImage.h"
#include "Maths.h"
#include "CCamera.h"
#include "CPlayer.h"

// �t���[���̉��̃T�C�Y
#define FRAME_SIZE_X (450.0f)
// �t���[���̏c�̃T�C�Y
#define FRAME_SIZE_Y (40.0f)
// �Q�[�W�̕�
#define FRAME_BORDER (2.0f)
// �o�[�̉��̃T�C�Y
#define BAR_SIZE_X (FRAME_SIZE_X - FRAME_BORDER*2.0f)
// �o�[�̏c�̃T�C�Y
#define BAR_SIZE_Y (FRAME_SIZE_Y - FRAME_BORDER*2.0f)

// �X�P�[���l�v�Z���̃J�����Ƃ̋����̍ŏ�
#define SCALE_DIST_MIN 0.0f
// �X�P�[���l�v�Z���̃J�����Ƃ̋����̍ő�
#define SCALE_DIST_MAX 300.0f
// �X�P�[���l�̍ŏ��l
#define SCALE_MIN 0.7f
// �X�P�[���l�̍ő�l
#define SCALE_MAX 1.5f

// �_���[�W�o�[���������n�߂�܂ł̑҂�����
#define DAMAGE_WAIT_TIME 1.0f
// �_���[�W�|�C���g��1��������̂ɂ����鎞��
#define DAMAGE_SUB_TIME 0.001f

// �R���X�g���N�^
CHpGauge::CHpGauge(bool is3dGauge)
	: mMaxValue(100)
	, mValue(100)
	, mDamageValue(100)
	, mCenterRatio(0.0f, 0.0f)
	, mIs3dGauge(is3dGauge)
	, mDamageWaitTime(0.0f)
	, mDamageElapsedTime(0.0f)
{
	mpFrameImage = new CImage("Frame");
	mpFrameImage->SetSize(FRAME_SIZE_X, FRAME_SIZE_Y);

	mpDamagaBarImage = new CImage("Gauge");
	mpDamagaBarImage->SetSize(BAR_SIZE_X, BAR_SIZE_Y);

	mpHpBarImage = new CImage("Gauge");
	mpHpBarImage->SetSize(BAR_SIZE_X, BAR_SIZE_Y);

	mpEdgeImage = new CImage("FrameEdge");
	mpEdgeImage->SetSize(FRAME_SIZE_X, FRAME_SIZE_Y);

	if (!mIs3dGauge)
	{
		mScale = 1.0f;
	}

	// �ŏ��͔�\��
	SetShow(false);
}

// �f�X�g���N�^
CHpGauge::~CHpGauge()
{
}

// HP�Q�[�W���폜
void CHpGauge::Kill()
{
	CTask::Kill();
	mpFrameImage->Kill();
	mpDamagaBarImage->Kill();
	mpHpBarImage->Kill();
	mpEdgeImage->Kill();
}

// �\�����邩�ǂ����ݒ�
void CHpGauge::SetShow(bool isShow)
{
	// �x�[�X�N���X�̕\���ݒ菈��
	CTask::SetShow(isShow);
	mpFrameImage->SetShow(isShow);
	mpDamagaBarImage->SetShow(isShow);
	mpHpBarImage->SetShow(isShow);
	mpEdgeImage->SetShow(isShow);
}

// �傫����ݒ�
void CHpGauge::SetScale(float scale)
{
	mSize = scale;
}

// �ő�l��ݒ�
void CHpGauge::SetMaxValue(int value)
{
	mMaxValue = value;
}

// ���ݒl
void CHpGauge::SetValue(int value)
{
	// �_���[�W���󂯂��ꍇ�́A�_���[�W�֘A�̃^�C�}�[�����Z�b�g
	if (value < mValue)
	{
		mDamageWaitTime = DAMAGE_WAIT_TIME;
		mDamageElapsedTime = 0.0f;
	}
	else
	{
		if (value > mDamageValue)
		{
			mDamageValue = value;
		}
	}

	mValue = value;
}

// ���S�ʒu�̂̊����̐ݒ�
void CHpGauge::SetCenterRatio(const CVector2& ratio)
{
	mCenterRatio = ratio;
}

// ���[���h���W��ݒ�
void CHpGauge::SetWorldPos(const CVector& worldPos)
{
	if (mIs3dGauge)
	{
		mScale = mSize;
	}
	// ���݂̃J�������擾
	CCamera* cam = CCamera::CurrentCamera();
	if (cam == nullptr)return;

	// �w�肳�ꂽ���[���h���W���X�N���[�����W�ɕϊ�
	CVector screenPos = cam->WorldToScreenPos(worldPos);
	
	// �ݒ胏�[���h���W���J�����̔w��ł���΁A
	// HP�Q�[�W��\�����Ȃ�
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
	
		// HP�Q�[�W���\��
		SetShow(true);
	}
	// �J�����̋����������ꍇ�́A��\��
	else
	{
		SetShow(false);
	}
}

// �X�V
void CHpGauge::Update()
{
	// ����HP���_���[�W�o�[�\���p��HP��菬�����ꍇ��
	if (mValue < mDamageValue)
	{
		// �_���[�W�|�C���g�����܂ł̑҂�����
		if (mDamageWaitTime > 0.0f)
		{
			mDamageWaitTime -= Time::DeltaTime();
		}
		// �_���[�W�|�C���g������
		else
		{
			// DAMAGE_SUB_TIME�̎��Ԃ��o�߂���x�ɁA�_���[�W�|�C���g1����������
			if (mDamageElapsedTime >= DAMAGE_SUB_TIME)
			{
				mDamageValue -= 5;
				if (mDamageValue < mValue)
				{
					mDamageValue = mValue;
				}
				mDamageElapsedTime -= DAMAGE_SUB_TIME;
			}
			mDamageElapsedTime += Time::DeltaTime();
		}
	}

	// �_���[�W�o�[�̃T�C�Y���_���[�W�|�C���g��HP�̍ő�l�Ŕ��f����
	float damageBer = Math::Clamp01((float)mDamageValue / mMaxValue);
	CVector2 damageSize = CVector2(BAR_SIZE_X * damageBer, BAR_SIZE_Y) * mScale;	
	mpDamagaBarImage->SetSize(damageSize);

	// �Q�[�W�̃t���[���o�[�̈ʒu��ݒ�
	mpFrameImage->SetPos(mPosition);
	CVector2 barPos = mPosition;
	barPos.X(barPos.X() - FRAME_SIZE_X * mCenterRatio.X() * mScale);
	mpDamagaBarImage->SetPos(barPos + CVector2(FRAME_BORDER, FRAME_BORDER) * mScale);
	mpHpBarImage->SetPos(barPos + CVector2(FRAME_BORDER, FRAME_BORDER) * mScale);
	mpEdgeImage->SetPos(mPosition);

	// �t���[���T�C�Y��ύX
	mpFrameImage->SetSize(CVector2(FRAME_SIZE_X, FRAME_SIZE_Y) * mScale);
	mpEdgeImage->SetSize(CVector2(FRAME_SIZE_X, FRAME_SIZE_Y) * mScale);

	// HP�o�[�̃T�C�Y���ő�l�ƌ��ݒl���狁�߂�
	float percent = Math::Clamp01((float)mValue / mMaxValue);
	CVector2 size = CVector2(BAR_SIZE_X * percent, BAR_SIZE_Y) * mScale;
	mpHpBarImage->SetSize(size);

    // �t���[���̒��S�ʒu��ݒ�
	mpFrameImage->SetCenter
	(
		FRAME_SIZE_X * mCenterRatio.X()* mScale,
		FRAME_SIZE_Y * mCenterRatio.Y() * mScale
	);

	// �_���[�W�o�[�̒��S�ʒu��ݒ�
	mpDamagaBarImage->SetCenter
	(
		0.0f,
		FRAME_SIZE_Y * mCenterRatio.Y() * mScale
	);

	// Hp�o�[�̒��S�ʒu��ݒ�
	mpHpBarImage->SetCenter
	(
		0.0f,
		FRAME_SIZE_Y * mCenterRatio.Y() * mScale
	);

	// �ӂ��̒��S�ʒu��ݒ�
	mpEdgeImage->SetCenter
	(
		FRAME_SIZE_X * mCenterRatio.X() * mScale,
		FRAME_SIZE_Y * mCenterRatio.Y() * mScale
	);
	
	// �_���[�W�o�[��ԐF�ɐݒ�
	mpDamagaBarImage->SetColor(CColor::red);

	// HP�̊����Ńo�[�̐F��ύX
	CColor color;
	// 10%�ȉ��͞�F
	if (percent <= 0.1f) color = CColor(1.0f, 0.6f, 0.0f);
	// 30%�ȉ��͉��F
	else if (percent <= 0.3f)color = CColor(1.0f, 1.0f, 0.0f);
	// ����ȊO�͗ΐF
	else color = CColor(0.0f, 1.0f,0.0f);
	// �o�[�ɐF��ݒ�
	mpHpBarImage->SetColor(color);

	// 3D��Ԃɔz�u����Q�[�W�́A�c���HP��0�Ȃ��\���ɂ���
	if (mIs3dGauge && mValue <= 0)
	{
		SetShow(false);
	}
}