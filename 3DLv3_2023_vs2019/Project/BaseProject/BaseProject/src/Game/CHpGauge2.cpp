#include "CHpGauge2.h"
#include "CBillBoardImage.h"
#include "CSlime.h"
#include "Maths.h"

// �Q�[�W�̃t���[���摜�̃t�@�C���p�X
#define FRAME_IMAGE "Character\\Player\\HP\\Frame.png"  //HP�t���[���摜
// �Q�[�W�̃o�[�摜�̃t�@�C���p�X
#define BAR_IMAGE "UI\\white.png"

// �t���[���̉��̃T�C�Y
#define FRAME_SIZE_X (15.0f)
// �t���[���̏c�̃T�C�Y
#define FRAME_SIZE_Y (5.0f)
// �΂̕�
#define FRAME_BORDER (2.0f)
// �o�[�̉��̃T�C�Y
#define BAR_SIZE_X (FRAME_SIZE_X - FRAME_BORDER*2.0f)
// �o�[�̏c�̃T�C�Y
#define BAR_SIZE_Y (FRAME_SIZE_Y - FRAME_BORDER*2.0f)


// �R���X�g���N�^
CHpGauge2::CHpGauge2()	
	:mMaxValue(100)
	, mValue(100)
{
	mpBarImage = new CBillBoardImage(BAR_IMAGE);
	//mpBarImage->SetSize(BAR_SIZE_X, BAR_SIZE_Y);

	//mpFrameImage = new CBillBoardImage(FRAME_IMAGE);
	//mpFrameImage->SetSize(size);
}

// �f�X�g���N�^
CHpGauge2::~CHpGauge2()
{

}

// �����ݒ�
void CHpGauge2::Setup(const CVector& pos, const CVector& dir)
{
	//�@�ړ������x�N�g����������]���ď�����֌�����
	CVector side = CVector::Cross(CVector::up, dir);
	CQuaternion q;
	q.RotationAxis(side, 75.0f);
	mMoveSpeed = q * dir * 75.0f;
}

// �ő�l��ݒ�
void CHpGauge2::SetMaxValue(int value)
{
	mMaxValue = value;
}

// ���ݒl
void CHpGauge2::SetValue(int value)
{
	mValue = value;
}

// �X�V
void CHpGauge2::Update()
{
	// �o�[�̃T�C�Y���ő�l�ƌ��ݒl���狁�߂�
	float percent = Math::Clamp01((float)mValue / mMaxValue);
	CVector2 size = CVector2(BAR_SIZE_X, BAR_SIZE_Y);
	size.X(BAR_SIZE_X * percent);
	mpBarImage->SetSize(size);

	// HP�̊����Ńo�[�̐F��ύX
	CColor color;
	// 10%�ȉ��͐ԐF
	if (percent <= 0.1f) color = CColor(1.0f, 0.0f, 0.0f);
	// 30%�ȉ��͉��F
	else if (percent <= 0.3f)color = CColor(1.0f, 1.0f, 0.0f);
	// ����ȊO�͗ΐF
	else color = CColor(0.0f, 1.0f, 0.0f);
	// �o�[�ɐF��ݒ�
	mpBarImage->SetColor(color);

	// �X���C���̏����擾
	CSlime* enemy = CSlime::Instance();
	// �X���C���̍��W
	CVector enemyPos = CVector::zero;
	// �X���C���܂ł̃x�N�g��
	CVector enemyVec = CVector::zero;
	if (enemy != nullptr)
	{
		enemyPos = enemy->Position()+CVector(0.0f,0.0f,0.0f);
		enemyVec = enemyPos + mPosition;
	}
	mpBarImage->Position(enemyVec);
}