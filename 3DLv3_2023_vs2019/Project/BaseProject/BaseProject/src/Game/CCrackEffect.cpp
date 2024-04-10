#include "CCrackEffect.h"
#include "CCrack.h"
#include "Maths.h"

CCrackEffect* CCrackEffect::spInstance = nullptr;

// �Ђъ���̊Ԋu����
#define THROW_INTERVAL 0.4f
// �Ђъ���̕����̃u����
#define FLAME_DIR_RAND 0.02f
// �Ђъ���̈ړ����x
#define FLAME_MOVE_SPEED 25.0f
// �Ђъ���̐F
#define FLAME_COLOR CColor(1.0f, 1.0f, 1.0f)

// �R���X�g���N�^
CCrackEffect::CCrackEffect(CObjectBase* owner, const CMatrix* attach,
	const CVector& offsetPos, const CMatrix& offsetRot)
	: mpOwner(owner)
	, mpAttachMtx(attach)
	, mThrowOffsetPos(offsetPos)
	, mThrowOffsetRot(offsetRot)
	, mElapsedTime(0.0f)
	, mIsThrowing(false)
{
	spInstance = this;
}

// �f�X�g���N�^
CCrackEffect::~CCrackEffect()
{
	spInstance = nullptr;
}

// �Ђъ�����J�n
void CCrackEffect::Start()
{
	mIsThrowing = true;
	mElapsedTime = 0.0f;
}

// �Ђъ�����~
void CCrackEffect::Stop()
{
	mIsThrowing = false;
}

// �Ђъ�������Ă��邩�ǂ���
bool CCrackEffect::IsThrowing() const
{
	return mIsThrowing;
}

// �Ђъ��ꎞ�̃I�t�Z�b�g�ʒu��ݒ�
void CCrackEffect::SetThrowOffsetPos(const CVector& pos)
{
	mThrowOffsetPos = pos;
}

// �Ђъ��ꎞ�̃I�t�Z�b�g��]�l��ݒ�
void CCrackEffect::SetThrowOffsetRot(const CMatrix& rot)
{
	mThrowOffsetRot = rot;
}

// �Ђъ���̈ʒu���擾
CVector CCrackEffect::GetThrowPos() const
{
	// �A�^�b�`����s�񂪐ݒ肳��Ă���ꍇ�́A�s��̍��W��Ԃ�
	if (mpAttachMtx != nullptr)
	{
		CVector pos = mpAttachMtx->Position();
		pos += mpAttachMtx->VectorX() * mThrowOffsetPos.X()
			+ mpAttachMtx->VectorY() * mThrowOffsetPos.Y()
			+ mpAttachMtx->VectorZ() * mThrowOffsetPos.Z();
		return pos;
	}
	// �����傪�ݒ肳��Ă���ꍇ�́A������̍��W��Ԃ�
	else if (mpOwner != nullptr)
	{
		return mpOwner->Position() + (mpOwner->Rotation() * mThrowOffsetPos);
	}

	return CVector::zero;
}

// �Ђъ���̕������擾
CVector CCrackEffect::GetThrowDir() const
{
	// �A�^�b�`����s�񂪐ݒ肳��Ă���ꍇ�́A�s��̐��ʕ����x�N�g����Ԃ�
	if (mpAttachMtx != nullptr)
	{
		return mThrowOffsetRot * mpAttachMtx->VectorZ();
	}
	// �����傪�ݒ肳��Ă���ꍇ�́A������̐��ʕ����x�N�g����Ԃ�
	else if (mpOwner != nullptr)
	{
		return mThrowOffsetRot * mpOwner->VectorZ();
	}

	return CVector::forward;
}

// �Ђъ�����쐬
void CCrackEffect::CreateSlash()
{
	// �Ђъ�����쐬
	CCrack* crack = new CCrack(ETag::eNone);

	// �ʒu���擾
	CVector pos = GetThrowPos();// + CVector(0.0f, 10.0f, 0.0f);
	// �������擾
	CVector dir = GetThrowDir();// + CVector(0.0f, -1.0f, 0.0f);
	dir.Normalize();
	// �ʒu�A�����A�ړ����x��ݒ�
	crack->Setup(pos, dir, FLAME_MOVE_SPEED);
	// �Ђъ���̃J���[��ݒ�
	crack->SetColor(FLAME_COLOR);
	// ���Z�u�����h�ɂ��āA�Ђъ��ꂪ�������Ă���悤�Ɍ�����
	crack->SetBlendType(EBlend::eAdd);
}

// �X�V
void CCrackEffect::Update()
{
	// �Ђъ���𔭎˂��Ă�����
	if (mIsThrowing)
	{
		// �o�ߎ��Ԃɉ����āA�Ђъ�����쐬
		if (mElapsedTime >= THROW_INTERVAL)
		{
			CreateSlash();
			mElapsedTime -= THROW_INTERVAL;
		}
		mElapsedTime += Time::DeltaTime();
	}
}