#include "CSlash.h"
#include "CSlashing.h"
#include "CInput.h"
#include "CPlayer.h"
#include "Maths.h"

CSlash* CSlash::spInstance = nullptr;

// �X���b�V���̔��ˊԊu����
#define THROW_INTERVAL 0.8f
// �X���b�V���̔��˕����̃u����
#define FLAME_DIR_RAND 0.02f
// �X���b�V���̈ړ����x
#define FLAME_MOVE_SPEED 75.0f
// �X���b�V���̐F
#define FLAME_COLOR CColor(0.0f, 1.0f, 0.9f)

// �R���X�g���N�^
CSlash::CSlash(CObjectBase* owner, const CMatrix* attach,
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
CSlash::~CSlash()
{
	spInstance = nullptr;
}

// �X���b�V���𔭎ˊJ�n
void CSlash::Start()
{
	mIsThrowing = true;
	mElapsedTime = 0.0f;
}

// �X���b�V���𔭎˒�~
void CSlash::Stop()
{
	mIsThrowing = false;
}

// �X���b�V���𔭎˂��Ă��邩�ǂ���
bool CSlash::IsThrowing() const
{
	return mIsThrowing;
}

// ���ˎ��̃I�t�Z�b�g�ʒu��ݒ�
void CSlash::SetThrowOffsetPos(const CVector& pos)
{
	mThrowOffsetPos = pos;
}

// ���ˎ��̃I�t�Z�b�g��]�l��ݒ�
void CSlash::SetThrowOffsetRot(const CMatrix& rot)
{
	mThrowOffsetRot = rot;
}

// �X���b�V���̔��ˈʒu���擾
CVector CSlash::GetThrowPos() const
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

// �X���b�V���̔��˕������擾
CVector CSlash::GetThrowDir() const
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

// �X���b�V�����쐬
void CSlash::CreateSlash()
{
	// �X���b�V�����쐬
	CSlashing* slash = new CSlashing(ETag::eSlash);

	// ���ˈʒu���擾
	CVector pos = GetThrowPos();// + CVector(0.0f, 10.0f, 0.0f);
	// ���˕������擾
	CVector dir = GetThrowDir();// + CVector(0.0f, -1.0f, 0.0f);
	// ���˕����������_���Ńu����
	//dir.X(dir.X() + Math::Rand(-FLAME_DIR_RAND, FLAME_DIR_RAND));
	//dir.Y(dir.Y() + Math::Rand(-FLAME_DIR_RAND, FLAME_DIR_RAND));
	//dir.Z(dir.Z() + Math::Rand(-FLAME_DIR_RAND, FLAME_DIR_RAND));
	dir.Normalize();
	// ���ˈʒu�A�����A�ړ����x��ݒ�
	slash->Setup(pos, dir, FLAME_MOVE_SPEED);

	// �v���C���[�̂�������֌���
	//CVector dir2 = player->Position() - slash->Position();
	//dir2.Y(0.0f);
	//dir2.Normalize();
	//slash->Rotation(CQuaternion::LookRotation(dir2));
	
	//CPlayer* player = CPlayer::Instance();
	//slash->Rotation(75.0f, 0.0f, 0.0f);
		//slash->Rotation(75.0f, 180.0f, 0.0f);
		//slash->Rotation(75.0f, 270.0f, 0.0f);
	slash->Rotation(75.0f, -270.0f, 0.0f);
	// �X���b�V���̃J���[��ݒ�
	slash->SetColor(FLAME_COLOR);
	// ���Z�u�����h�ɂ��āA�X���b�V�����������Ă���悤�Ɍ�����
	slash->SetBlendType(EBlend::eAdd);
}

// �X�V
void CSlash::Update()
{
	// �X���b�V���𔭎˂��Ă�����
	if (mIsThrowing)
	{
		// �o�ߎ��Ԃɉ����āA�X���b�V�����쐬
		if (mElapsedTime >= THROW_INTERVAL)
		{
			CreateSlash();
			mElapsedTime -= THROW_INTERVAL;
		}
		mElapsedTime += Time::DeltaTime();
	}
}