#include "CEnemyLevelUI.h"
#include "CText.h"
#include "Maths.h"

// �X�P�[���l�v�Z���̃J�����Ƃ̋����̍ŏ�
#define SCALE_DIST_MIN 0.0f
// �X�P�[���l�v�Z���̃J�����Ƃ̋����̍ő�
#define SCALE_DIST_MAX 140.0f
// �X�P�[���l�̍ŏ��l
#define SCALE_MIN 0.7f
// �X�P�[���l�̍ő�l
#define SCALE_MAX 1.5f

#define SHADOW_COUNT 8     // �e�̃J�E���g
#define SHADOW_WIDTH 2.0f  // �e�̕�

// �R���X�g���N
CEnemyLevelUI::CEnemyLevelUI(bool is3dGauge)
	: mBasePos(100.0f + SHADOW_WIDTH, 32.0f)
	, mCenterRatio(0.0f, 0.0f)
	, mIs3dGauge(is3dGauge)
{
	CVector2 size = CVector2(WINDOW_WIDTH, WINDOW_HEIGHT);
	ETextAlignH textAlignH = ETextAlignH::eLeft;

	// ���x���̃e�L�X�g���쐬
	mpLevelText = new CText
	(
		nullptr, 24,
		mBasePos,
		size,
		CColor(1.0f, 1.0f, 1.0f),
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpLevelText->SetTextAlignH(textAlignH);
}

// �f�X�g���N�^
CEnemyLevelUI::~CEnemyLevelUI()
{
	SAFE_DELETE(mpLevelText);
}

// �G�l�~�[�̃��x����ݒ�
void CEnemyLevelUI::SetEnemyLevel(std::string level)
{
	mpLevelText->SetText(level.c_str());
}

// ���S�ʒu�̂̊����̐ݒ�
void CEnemyLevelUI::SetCenterRatio(const CVector2& ratio)
{
	mCenterRatio = ratio;
}

// ���[���h���W��ݒ�
void CEnemyLevelUI::SetWorldPos(const CVector& worldPos)
{
	// ���݂̃J�������擾
	CCamera* cam = CCamera::CurrentCamera();
	if (cam == nullptr)return;

	// �w�肳�ꂽ���[���h���W���X�N���[�����W�ɕϊ�
	CVector screenPos = cam->WorldToScreenPos(worldPos);

	// �ݒ胏�[���h���W���J�����̔w��ł���΁A
	// �\�����Ȃ�
	if (screenPos.Z() < 0.0f)
	{
		SetShow(false);
		return;
	}

	// ���Ƃ߂��X�N���[�����W�����g�̈ʒu�ɐݒ�
	mPosition = (CVector2)screenPos + CVector2(-85.0f, -32.0f);

	// �ݒ肳�ꂽ���[���h���W�ƃJ�����̍��W�����߂�
	float dist = (worldPos - cam->Position()).Length();

	if (dist <= SCALE_DIST_MAX)
	{
		// �J�������痣��邲�ƂɃX�P�[���l������������
		float ratio = 0.3f - Math::Clamp01((dist - SCALE_DIST_MIN) / (SCALE_DIST_MAX - SCALE_DIST_MIN));
		mScale = Math::Lerp(SCALE_MIN, SCALE_MAX, ratio);

		// �\��
		SetShow(true);
	}
	// �J�����̋����������ꍇ�́A��\��
	else
	{
		SetShow(false);
	}
}

// �`��
void CEnemyLevelUI::Render()
{
	if (mIs3dGauge)
	{
		mpLevelText->SetColor(0.0f, 0.0f, 0.0f);

		for (int i = 0; i < SHADOW_COUNT; i++)
		{
			CVector2 v = CVector2(1.0f, 0.0f);
			float alpha = (float)i / SHADOW_COUNT;
			CQuaternion rot = CQuaternion(0.0f, 0.0f, 360.0f * alpha);
			mpLevelText->SetPos(mPosition + (CVector2)(rot * v) * SHADOW_WIDTH);
			mpLevelText->Render();
		}
		mpLevelText->SetColor(1.0f, 1.0f, 1.0f);
		mpLevelText->SetPos(mPosition);
		mpLevelText->Render();
	}
}