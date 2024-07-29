//�v���C���[�N���X�̃C���N���[�h
#include "CPlayer.h"
#include "CInput.h"
#include "CCamera.h"
#include "CGameCamera2.h"
#include "CHpGauge.h"
#include "CSpGauge.h"
#include "CExpGauge.h"
#include "CAvoidanceGauge.h"
#include "Maths.h"
#include "CSword.h"
#include "CShield.h"
#include "CShieldRotate.h"
#include "CSlash.h"
#include "CHealCircle.h"
#include "CBuffCircle.h"
#include "CBuffAura.h"
#include "CPowerUpAura.h"
#include "CGamePlayerUI.h"
#include "CGameOverScene.h"

// �v���C���[�̃C���X�^���X
CPlayer* CPlayer::spInstance = nullptr;
int CPlayer::mHp;
int CPlayer::mMaxHp;
int CPlayer::mSp;
int CPlayer::mRecoveryCount;
bool CPlayer::mHeal;
bool CPlayer::mRolling;

// �v���C���[�̃��f���f�[�^�̃p�X
#define MODEL_PATH "Character\\Player\\player.x"

// �v���C���[�̃A�j���[�V�����f�[�^�̃e�[�u��
const CPlayer::AnimData CPlayer::ANIM_DATA[] =
{
	{ "Character\\Player\\animation\\DogIdle.x",	   true,	221.0f,	 1.0f},  // �ҋ@ 221.0f
	{ "Character\\Player\\animation\\DogWalk.x",	   true,	 69.0f,	 1.0f},  // ���s
	{ "Character\\Player\\animation\\DogAttack.x",	   false,    91.0f,	 1.0f},  // �U�� 91.0f
	{ "Character\\Player\\animation\\DogJump.x",	   true,	 49.0f,	 1.0f},  // �W�����v
	{ "Character\\Player\\animation\\DogAttack2.x",	   false,	140.0f,	 1.0f},  // �U��2
	{ "Character\\Player\\animation\\DogAttack3.x",	   false,	 91.0f,	 1.0f},  // �U��3
	{ "Character\\Player\\animation\\DogAttack4.x",	   false,	105.0f,	 1.0f},  // �U��4 105.0f
	{ "Character\\Player\\animation\\DogAttack5.x",	   false,	101.0f,	 1.0f},  // �U��5 101.0f
	{ "Character\\Player\\animation\\DogAttack6.x",	   false,	219.0f,	 1.0f},  // �U��6
	{ "Character\\Player\\animation\\DogAttack7.x",	   false,	190.0f,	 1.0f},  // �U��7 213.0f
	{ "Character\\Player\\animation\\DogPowerUp.x",	   false,	143.0f,  4.0f},  // �U���̓A�b�v
	{ "Character\\Player\\animation\\DogHit.x",	       false,	 43.0f,	 1.0f},  // �q�b�g 43.0f
	{ "Character\\Player\\animation\\DogGuard.x",	   false,	 47.0f,	 1.0f},  // �K�[�h 47.0f
	{ "Character\\Player\\animation\\DogGuardHit.x",   false,	 47.0f,	 1.0f},  // �K�[�h�q�b�g 47.0f
	{ "Character\\Player\\animation\\DogRolling.x",	   false,	 43.0f,	 1.0f},  // ��� 43.0f
	//{ "Character\\Player\\animation\\DogImpact.x",   true,	 43.0f,	 1.0f},  // �Ռ�
	{ "Character\\Player\\animation\\DogDie.x",	       false,	235.0f,	 1.0f},  // ����
	{ "Character\\Player\\animation\\DogJumpAttack.x", false,	172.0f,	 1.0f},  // �W�����v�U��
};

#define PLAYER_HEIGHT  1.2f  // �����R���C�_�[
#define JUMP_SPEED     1.2f  // �W�����v�X�s�[�h
#define GRAVITY     0.0625f  // �d��
#define JUMP_END_Y     1.0f  // �W�����v�I��
#define MOVE_SPEED     0.3f  // �ړ����x
#define DEFAULT_SCALE 10.0f  // �f�t�H���g�̃X�P�[���l

bool CPlayer::IsDeath() const
{
	return mCharaStatus.hp <= 0;
}

bool CPlayer::IsDeath2() const
{
	return mCharaStatus.level <= 0;
}

bool CPlayer::IsDeath3() const
{
	return mCharaStatus.power <= 0;
}

// �R���X�g���N�^
CPlayer::CPlayer()
	: CXCharacter(ETag::ePlayer, ETaskPriority::ePlayer)
	, mState(EState::eIdle)
	, mpRideObject(nullptr)
	, mRollingTime(0)
	, mRollingCount(1)
	, mAttackTime(0)
	, mAttackCount(0)
	, mHealCount(0)
	, mRecoveryAmount(0)
	, mStateStep(0)
	, mStateJumpAttackStep(0)
	, mBuffStep(0)
	, mDefaultPos(CVector::zero)
	, mIsPlayedSlashSE(false)
	, mIsSpawnedSlashEffect(false)
	, mIsGrounded(false)
	, mMoveSpeed(CVector::zero)
{
	// �C���X�^���X�̐ݒ�
	spInstance = this;
	mRecoveryCount = 0;
	mHp = 0;
	mMaxHp = 0;
	mDefenseUp = false;
	mHeal = false;
	mPowerUp = false;
	mElapsedDefenseUpTime = 0.0f;
	mElapsedPowerUpTime = 0.0f;
	mRolling = false;

	// ���f���f�[�^�ǂݍ���
	CModelX* model = CResourceManager::Get<CModelX>("Player");
	// �f�t�H���g�X�P�[���̔��f
	Scale(CVector::one * DEFAULT_SCALE);

	// �v���C���[�����쐬
	mpGameUI = new CGamePlayerUI();

	// Lv.��\��
	std::string lv = "Lv.";
	mpGameUI->SetLevel(lv);

	// ���O��ݒ�
	std::string name = "Dog Knight";
	mpGameUI->SetPlayerName(name);

	// Hp��\��
	std::string hp = "HP";
	mpGameUI->SetHpText(hp);

	// Sp��\��
	std::string sp = "SP";
	mpGameUI->SetSpText(sp);

	// Exp��\��
	std::string exp = "EXP";
	mpGameUI->SetExpText(exp);

	int lockLevel = 5;
	std::to_string(lockLevel);
	std::string LockLevel = "���x��" + std::to_string(lockLevel);
	mpGameUI->SetLockLevel(LockLevel);

	int lockLevel2 = 10;
	std::to_string(lockLevel2);
	std::string LockLevel2 = "���x��" + std::to_string(lockLevel2);
	mpGameUI->SetLockLevel2(LockLevel2);

	// �ŏ���1���x���ɐݒ�
	ChangeLevel(1);

	// �e�[�u�����̃A�j���[�V�����f�[�^��ǂݍ���
	int size = ARRAY_SIZE(ANIM_DATA);
	for (int i = 0; i < size; i++)
	{
		const AnimData& data = ANIM_DATA[i];
		if (data.path.empty()) continue;
		model->AddAnimationSet(data.path.c_str());
	}
	// CXCharacter�̏�����
	Init(model);

	// �ŏ��͑ҋ@�A�j���[�V�������Đ�
	ChangeAnimation(EAnimType::eIdle);

	mpColliderLine = new CColliderLine
	(
		this, ELayer::ePlayer,
		CVector(0.0f, -0.01f, 0.0f),
		CVector(0.0f, PLAYER_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });
	mpColliderLine->Position(0.0f, 0.2f, 0.0f)
		;
	// �L�����N�^�[���m�̉����߂��R���C�_�[(��)
	mpColliderSphereHead = new CColliderSphere
	(
		this,ELayer::ePlayer,
		0.35f
	);
	mpColliderSphereHead->SetCollisionLayers({ ELayer::eEnemy, ELayer::ePortion });
	mpColliderSphereHead->SetCollisionTags({ ETag::eEnemy, ETag::ePortionBlue,
		                                     ETag::ePortionGreen, ETag::ePortionRed });
	mpColliderSphereHead->Position(0.0f, 0.1f, 0.03f);

	// �L�����N�^�[���m�̉����߂��R���C�_�[(��)
	mpColliderSphereBody = new CColliderSphere
	(
		this, ELayer::ePlayer,
		0.35f
	);
	mpColliderSphereBody->SetCollisionLayers({ ELayer::eEnemy, ELayer::ePortion });
	mpColliderSphereBody->SetCollisionTags({ ETag::eEnemy, ETag::ePortionBlue,
		                                     ETag::ePortionGreen, ETag::ePortionRed });

	///�_���[�W���󂯂�R���C�_�[���쐬(��)
	mpDamageColHead = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.37f,
		false
	);
	//�_���[�W���󂯂�R���C�_�[��
	//�Փ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageColHead->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColHead->SetCollisionTags({ ETag::eEnemy,  ETag::eFlame ,ETag::eWave,ETag::eLightningBall,
	                                    ETag::eImpact, ETag::eHomingBall, ETag::eDrain,
		                                ETag::eWeapon });
	//�_���[�W���󂯂�R���C�_�[��������ւ��炷
	mpDamageColHead->Position(0.0f, 0.1f, 0.03f);

	///�_���[�W���󂯂�R���C�_�[���쐬(��)
	mpDamageColBody = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.36f,
		false
	);
	//�_���[�W���󂯂�R���C�_�[��
	//�Փ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageColBody->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageColBody->SetCollisionTags({ ETag::eEnemy , ETag::eFlame, ETag::eWave,ETag::eLightningBall,
		                                ETag::eImpact, ETag::eHomingBall, ETag::eDrain,
		                                ETag::eWeapon });

	//�f�t�H���g���W��ݒ�
	mDefaultPos = Position();

	// �L�����N�^�[�̉����߂��R���C�_�[��
	// �_���[�W���󂯂�R���C�_�[���v���C���[�̓��̍s��ɃA�^�b�`
	const CMatrix* headMty = GetFrameMtx("Armature_mixamorig_Head");
	mpColliderSphereHead->SetAttachMtx(headMty);
	mpDamageColHead->SetAttachMtx(headMty);

	// �L�����N�^�[�̉����߂��R���C�_�[��
	// �_���[�W���󂯂�R���C�_�[���v���C���[�̑̂̍s��ɃA�^�b�`
	const CMatrix* bodyMty = GetFrameMtx("Armature_mixamorig_Spine");
	mpColliderSphereBody->SetAttachMtx(bodyMty);
	mpDamageColBody->SetAttachMtx(bodyMty);

	//���𐶐����ĉE��Ɏ�������
	mpSword = new CSword();
	mpSword->SetAttachMtx(GetFrameMtx("Armature_mixamorig_RightHand"));
	mpSword->SetOwner(this);

	//���𐶐����č���Ɏ�������
	mpShield = new CShield();
	mpShield->SetAttachMtx(GetFrameMtx("Armature_mixamorig_LeftHand"));
	mpShield->SetOwner(this);

	// �V�[���h�Ƃ̋���
	float ShieldDist = 8.0f;
	// ��]����V�[���h
	mpShieldRotate = new CShieldRotate(0.0f, ShieldDist);
	mpShieldRotate->SetOwner(this);

	// ��]����V�[���h2
	mpShieldRotate2 = new CShieldRotate(180.0, ShieldDist);
	mpShieldRotate2->SetOwner(this);

	// ��]����V�[���h3
	mpShieldRotate3 = new CShieldRotate(-270.0f, ShieldDist);
	mpShieldRotate3->SetOwner(this);

	// ��]����V�[���h4
	mpShieldRotate4 = new CShieldRotate(270.0f, ShieldDist);
	mpShieldRotate4->SetOwner(this);

	// �񕜃T�[�N��
	mpHealCircle = new CHealCircle();
	mpHealCircle->SetColor(CColor(0.0f, 1.0f, 0.0f));
	mpHealCircle->SetShow(false);
	mpHealCircle->SetOwner(this);

	// �o�t�T�[�N��(�ԐF)
	mpBuffCircle = new CBuffCircle();
	mpBuffCircle->SetColor(CColor(1.0f, 0.0f, 0.0f));
	mpBuffCircle->SetOwner(this);

	// �o�t�I�[��
	mpBuffAura = new CBuffAura();
	mpBuffAura->SetColor(CColor(1.0f, 0.0f, 0.0f));
	mpBuffAura->SetOwner(this);

	mpSlashSE = CResourceManager::Get<CSound>("SlashSound");
}

// �f�X�g���N�^
CPlayer::~CPlayer()
{
	// �����R���C�_�[
	SAFE_DELETE(mpColliderLine);
	// �L�����N�^�[�̉����߂��R���C�_�[
	SAFE_DELETE(mpColliderSphereHead);
	SAFE_DELETE(mpColliderSphereBody);
	// �_���[�W���󂯂�R���C�_�[
	SAFE_DELETE(mpDamageColHead);
	SAFE_DELETE(mpDamageColBody);

	// ����֘A�̍폜
	mpSword->Kill();
	mpShield->Kill();
	// ��]����V�[���h
	mpShieldRotate->Kill();
	mpShieldRotate2->Kill();
	mpShieldRotate3->Kill();
	mpShieldRotate4->Kill();

	// UI���폜
	mpGameUI->Kill();
}

// �C���X�^���X
CPlayer* CPlayer::Instance()
{
	return spInstance;
}

// �A�j���[�V�����؂�ւ�
void CPlayer::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// ��Ԃ̐؂�ւ�
void CPlayer::ChangeState(EState state)
{
	if (mState == state) return;
	mState = state;
	mStateStep = 0;
	mStateJumpAttackStep = 0;
}

// �ҋ@
void CPlayer::UpdateIdle()
{
	// �ڒn���Ă���΁A
	if (mIsGrounded)
	{
		if (mAttackTime >= 300 ||mAttackCount <=0)
		{
			mAttackCount = 0;
			mAttackTime = 0;
		}
		
		// �X�y�V�����U��
		if (mCharaStatus.level >= 10 && mCharaStatus.SpecialPoint >= 5)
		{
			if (CInput::PushKey('E'))
			{
				mCharaStatus.SpecialPoint -= 5;
				ChangeState(EState::eAttack4);
			}
		}

		if (mCharaStatus.SpecialPoint >= 4)
		{
			if (CInput::PushKey('Q'))
			{
				if (mDefenseUp != true || mPowerUp != true || mCharaStatus.hp < mCharaMaxStatus.hp)
				{
					mCharaStatus.SpecialPoint -= 4;
					ChangeState(EState::ePowerUp);
				}
			}
		}

		if (CInput::Key(VK_RBUTTON))
		{
			ChangeState(EState::eGuard);
		}
		if (CInput::PushKey(VK_SPACE))
		{
			ChangeState(EState::eJumpStart);
		}
	}
}

// ���s
void CPlayer::UpadateWalk()
{
	ChangeAnimation(EAnimType::eWalk);
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle);
	}
}

// �U��
void CPlayer::UpdateAttack()
{
	// �U���A�j���[�V�������J�n
	ChangeAnimation(EAnimType::eAttack);
	if (mAnimationFrame >= 35.0f)
	{
		//���ɍU���J�n��`����
		mpSword->AttackStart();

		// �U���I���҂���Ԃֈڍs
		ChangeState(EState::eAttackWait);
	}

	// �a��SE�̍Đ��ς݃t���O��������
	mIsPlayedSlashSE = false;
}

// �U��2
void CPlayer::UpdateAttack2()
{
	// �U���A�j���[�V�������J�n
	ChangeAnimation(EAnimType::eAttack2);

	// �U���I���҂���Ԃֈڍs
	ChangeState(EState::eAttackWait);

	//���ɍU���J�n��`����
	mpSword->AttackStart();
}

// �U��3
void CPlayer::UpdateAttack3()
{
	// �U���A�j���[�V�������J�n
	ChangeAnimation(EAnimType::eAttack3);
	// �U���I���҂���Ԃֈڍs
	ChangeState(EState::eAttackWait);

	//���ɍU���J�n��`����
	mpSword->AttackStart();
}

// �U��4
void CPlayer::UpdateAttack4()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);

	switch (mStateStep)
	{
		// �X�e�[�W0 : �U���A�j���[�V�������J�n
	case 0:
		ChangeAnimation(EAnimType::eAttack4);
		mStateStep++;
		break;
	case 1:
		if (mAnimationFrame >= 39.0f)
		{
			// �a���G�t�F�N�g�̐����ς݃t���O��������
			mIsSpawnedSlashEffect = false;
			// �a���G�t�F�N�g�𐶐����Ă��Ȃ����A�A�j���[�V������39%�ȏ�i�s������A
			if (!mIsSpawnedSlashEffect && GetAnimationFrameRatio() >= 0.39f)
			{
				// �a���G�t�F�N�g�𐶐����āA���ʕ����֔�΂�
				CSlash* slash = new CSlash
				(
					this,
					Position() + CVector(0.0f, 10.0f, 0.0f),
					VectorZ(),
					100.0f,
					100.0f
				);
				// �a���G�t�F�N�g�̐F�ݒ�
				slash->SetColor(CColor(0.15f, 0.5f, 0.5f));
				slash->SetOwner(this);
				if (mCharaStatus.level >= 50)
				{
					slash->Scale(2.0f, 2.0f, 2.0f);
				}
				if (mCharaStatus.level >= 100)
				{
					slash->Scale(3.0f, 3.0f, 3.0f);
				}

				mStateStep++;
				SetAnimationSpeed(1.5f);
				mIsSpawnedSlashEffect = true;
			}
		}
		break;
	case 2:
		if (mAnimationFrame >= 100.0f)
		{
			SetAnimationSpeed(1.0f);
			// �U���I���҂���Ԃֈڍs
			ChangeState(EState::eAttackWait);
		}
		break;
	}
}

// �U��5
void CPlayer::UpdateAttack5()
{
	switch (mStateStep)
	{
		// �U���A�j���[�V�������J�n
	case 0:
		ChangeAnimation(EAnimType::eAttack5);
		mStateStep++;
		break;
	case 1:
		if (mAnimationFrame >= 40.0f)
		{
			//���ɍU���J�n��`����
			mpSword->AttackStart();
			mStateStep++;
		}
		break;
	case 2:
		if (mAnimationFrame >= 90.0f)
		{
			//���ɍU���J�n��`����
			mpSword->AttackEnd();
			mStateStep++;
		}
		break;
	case 3:
		if (mAnimationFrame >= 99.0f)
		{
			// �U���I���҂���Ԃֈڍs
			ChangeState(EState::eAttackWait);
		}
		break;
	}
}

// �U��6
void CPlayer::UpdateAttack6()
{
	// �U���A�j���[�V�������J�n
	ChangeAnimation(EAnimType::eAttack6);
	// �U���I���҂���Ԃֈڍs
	ChangeState(EState::eAttackWait);

	//���ɍU���J�n��`����
	mpSword->AttackStart();
}

// �U��7
void CPlayer::UpdateAttack7()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	// �U���A�j���[�V�������J�n
	ChangeAnimation(EAnimType::eAttack7);

	if (mAnimationFrame >= 30.0f&& mAnimationFrame <= 31.0f)
	{
		mpSword->AttackStart();
	}
	else if (mAnimationFrame == 31.0f)
	{
		//���ɍU���I����`����
		mpSword->AttackEnd();
	}
	else if (mAnimationFrame == 52.0f)
	{
		mMoveSpeed.X(0.0f);
		mMoveSpeed.Z(0.0f);
	}
	if (mAnimationFrame >= 80.0f)
	{
		// �U���I���҂���Ԃֈڍs
		ChangeState(EState::eAttackWait);

		//���ɍU���J�n��`����
		mpSword->AttackStart();
	}
	else if (mAnimationFrame == 81.0f)
	{
		//���ɍU���I����`����
		mpSword->AttackEnd();
	}
}

// �U���I���҂�
void CPlayer::UpdateAttackWait()
{
	SetAnimationSpeed(1.0f);
	// �a��SE���Đ����Ă��Ȃ����A�A�j���[�V������25%�ȏ�i�s������A
	if (!mIsPlayedSlashSE && GetAnimationFrameRatio() >= 0.25f)
	{
		// �a��SE���Đ�
		mpSlashSE->Play();
		mIsPlayedSlashSE = true;
	}

	// �U���A�j���[�V�������I��������A
	if (IsAnimationFinished())
	{
		// �ҋ@��Ԃֈڍs
		ChangeState(EState::eIdle);
		ChangeAnimation(EAnimType::eIdle);

		//���ɍU���I����`����
		mpSword->AttackEnd();
	}
}

// �W�����v�J�n
void CPlayer::UpdateJumpStart()
{
	ChangeAnimation(EAnimType::eJumpStart);
	ChangeState(EState::eJump);

	if (mState != EState::eJumpAttack)
	{
		mpDamageColHead->SetEnable(true);
		mpDamageColBody->SetEnable(true);
		mMoveSpeed += CVector(0.0f, JUMP_SPEED, 0.0f);
	}
	mIsGrounded = false;
}

// �W�����v��
void CPlayer::UpdateJump()
{
	if (CInput::PushKey(VK_LBUTTON))
	{
		ChangeState(EState::eJumpAttack);
	}
	if (IsAnimationFinished())
	{
		ChangeState(EState::eJumpEnd);
	}
}

// �W�����v�I��
void CPlayer::UpdateJumpEnd()
{
	// �W�����v�A�j���[�V�������I�����A
	// �n�ʂɐڒn������A�ҋ@��Ԃ֖߂�
	if (IsAnimationFinished() && mIsGrounded)
	{
		ChangeState(EState::eIdle);
	}
}

// �ړ��̍X�V����
void CPlayer::UpdateMove()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	int speed = 1.5f;
	int speed2 = -1.5f;

	// �ړ�����
	// �L�[�̓��̓x�N�g�����擾
	CVector input;
	if (CInput::Key('W'))
	{
		input.Z(speed2);
		// SPACE�L�[�ŉ��
		if (CInput::PushKey(VK_SHIFT) && mRollingCount >= 1)
		{
			RollingCount();
		}
	}
	else if (CInput::Key('S'))
	{
		input.Z(speed);
		if (CInput::PushKey(VK_SHIFT) && mRollingCount >= 1)
		{
			RollingCount();
		}
	}
	if (CInput::Key('A'))
	{
		input.X(speed2);
		if (CInput::PushKey(VK_SHIFT) && mRollingCount >= 1)
		{
			RollingCount();
		}
	}
	else if (CInput::Key('D'))
	{
		input.X(speed);
		if (CInput::PushKey(VK_SHIFT) && mRollingCount >= 1)
		{
			RollingCount();
		}
	}

	// ���̓x�N�g���̒����œ��͂���Ă��邩����
	if (input.LengthSqr() > 0.0f)
	{
		// �J�����̌����ɍ��킹���ړ��x�N�g���ɕϊ�
		CCamera* mainCamera = CCamera::MainCamera();
		CVector camForward = mainCamera->VectorZ();
		CVector camSide = CVector::Cross(CVector::up, camForward);
		CVector move = camForward * input.Z() + camSide * input.X();
		move.Y(0.0f);
		move.Normalize();

		mMoveSpeed += move * MOVE_SPEED * mCharaStatus.mobility;

		// �ҋ@��Ԃł���΁A���s�A�j���[�V�����ɐ؂�ւ�
		if (mState == EState::eIdle)
		{
			// ���s�A�j���[�V�����ɐ؂�ւ�
			ChangeAnimation(EAnimType::eWalk);
			mpDamageColHead->SetEnable(true);
			mpDamageColBody->SetEnable(true);
		}
	}
	// �ړ��L�[����͂��Ă��Ȃ�
	else
	{
		// �ҋ@��Ԃł���΁A�ҋ@�A�j���[�V�����ɐ؂�ւ�
		if (mState == EState::eIdle)
		{
			// �ҋ@�A�j���[�V�����ɐ؂�ւ�
			ChangeAnimation(EAnimType::eIdle);
			mpDamageColHead->SetEnable(true);
			mpDamageColBody->SetEnable(true);
		}
	}

	if (mState != EState::eJumpStart && mState != EState::eJump && mState != EState::eJumpAttack && mState != EState::eJumpEnd)
	{
		// ���N���b�N�ōU����Ԃֈڍs
		if (CInput::PushKey(VK_LBUTTON))
		{
			mAttackCount++;
			mMoveSpeed.X(0.0f);
			mMoveSpeed.Z(0.0f);

			ChangeState(EState::eAttack);
			if (mAttackCount == 2)
			{
				mMoveSpeed.X(0.0f);
				mMoveSpeed.Z(0.0f);
				ChangeState(EState::eAttack5);
			}
			if (mAttackCount == 3)
			{
				mMoveSpeed.X(0.0f);
				mMoveSpeed.Z(0.0f);
				ChangeState(EState::eAttack3);
			}
		}
	}
}


// �U���̓A�b�v
void CPlayer::UpdatePowerUp()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	SetAnimationSpeed(1.5f);
	ChangeAnimation(EAnimType::ePowerUp);
	int rand = Math::Rand(0, 100);
	if (rand <= 40 || mPowerUp == true && mDefenseUp == true)
	{
		mHeal = true;
	}
	else if (rand > 40 && rand <=70)
	{
		if (mPowerUp == false)
		{
			mPowerUp = true;
		}
		else
		{
			mDefenseUp = true;
		}
	}
	else if (rand > 70)
	{
		if (mDefenseUp == false)
		{
			mDefenseUp = true;
		}
		else
		{
			mPowerUp = true;
		}
	}
	// �U���̓A�b�v�I��
	ChangeState(EState::ePowerUpEnd);
}

// �U���̓A�b�v�̏I��
void CPlayer::UpdatePowerUpEnd()
{
	// �U���A�b�v�̃A�j���[�V�������I��������A
	if (IsAnimationFinished())
	{
		SetAnimationSpeed(1.0f);
		// �ҋ@��Ԃֈڍs
		ChangeState(EState::eIdle);
	}
}

// �q�b�g
void CPlayer::UpdateHit()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	SetAnimationSpeed(1.0f);
	ChangeAnimation(EAnimType::eHit);
	if (IsAnimationFinished())
	{
		if (mCharaStatus.hp <= 0)
		{
			ChangeState(EState::eDie);
		}
		else
		{
			ChangeState(EState::eIdle);
		}
	}
}

// �K�[�h
void CPlayer::UpdateGuard()
{
	if (CInput::Key(VK_RBUTTON))
	{
		mMoveSpeed.X(0.0f);
		mMoveSpeed.Z(0.0f);
		ChangeAnimation(EAnimType::eGuard);
	}
	else
	{
		ChangeState(EState::eIdle);
	}
}

// �K�[�h���̃q�b�g
void CPlayer::UpdateGuardHit()
{
	ChangeAnimation(EAnimType::eGuardHit);

	if (CInput::Key('G'))
	{
		ChangeState(EState::eGuard);
	}
	else
	{
		ChangeState(EState::eIdle);
	}
}

// ���
void CPlayer::UpdateRolling()
{
	ChangeAnimation(EAnimType::eRolling);
	mRolling = true;
	Position(Position() + mMoveSpeed * 60.0f * Time::DeltaTime());
	RollingCount();
	if (IsAnimationFinished())
	{
		mRolling = false;
		ChangeState(EState::eIdle);
	}
	else
	{
		mpDamageColHead->SetEnable(false);
		mpDamageColBody->SetEnable(false);
	}
}

// ����
void CPlayer::UpdateDei()
{
	ChangeAnimation(EAnimType::eDie);
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);

	if (IsAnimationFinished())
	{
		CGameOverScene* gameOver = CGameOverScene::Instance();
		if (!gameOver->IsOpened())
		{
			gameOver->Open();
		}
	}
}

// ����
void CPlayer::UpdateRevival()
{
	mCharaStatus.hp = mCharaMaxStatus.hp;
	mCharaStatus.SpecialPoint = mCharaMaxStatus.SpecialPoint;
	ChangeState(EState::eIdle);
}

// �W�����v�U��
void CPlayer::UpdateJumpAttack()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eJumpAttack);
	if (mAnimationFrame >= 10.0f && mAnimationFrame < 45.0f)
	{
		mMoveSpeed.Y(0.0f);
	}
	else if (mAnimationFrame >= 45.0f)
	{
		mMoveSpeed -= CVector(0.0f, 0.0825, 0.0f);
	}

	switch (mStateJumpAttackStep)
	{
	case 0:
		if (mAnimationFrame >= 50.0f)
		{
			//���ɍU���J�n��`����
			mpSword->AttackStart();
			mStateJumpAttackStep++;
		}
		break;
	case 1:
		if (mAnimationFrame > 70.0f)
		{
			mpSword->AttackEnd();
			mStateJumpAttackStep++;
		}
		break;
	case 2:
		if (mAnimationFrame >= 90.0f)
		{
			SetAnimationSpeed(2.0f);
			mStateJumpAttackStep++;
		}
		break;
	case 3:
		if (mAnimationFrame >= 170.0f)
		{
			ChangeState(EState::eJumpAttackWait);
		}
		break;
	}
}

// �W�����v�U���I���҂�
void CPlayer::UpdateJumpAttackWait()
{
	// �U���A�j���[�V�������I��������A
	if (IsAnimationFinished()|| mIsGrounded==false)
	{
		SetAnimationSpeed(1.0f);
		//���ɍU���I����`����
		mpSword->AttackEnd();

		// �ҋ@��Ԃֈڍs
		ChangeState(EState::eIdle);
	}
}

// ���݂̌o���l������ă��x���A�b�v
void CPlayer::LevelUp()
{
	// ���݂̃��x��
	int level = mCharaStatus.level;
	// ���݂̍ő�X�e�[�^�X
	CharaStatus current = PLAYER_STATUS[level - 1];
	// ���̃��x���܂ł̌o���l�����܂��Ă�����
	while (mCharaStatus.exp >=current.exp)
	{
		// �o���l������āA���x�������Z
		mCharaStatus.exp -= current.exp;
		level++;
		// ���̃��x���̍ő�X�e�[�^�X��ݒ�
		current = PLAYER_STATUS[level - 1];
	}
	// ���x����؂�ւ�
	ChangeLevel(level);
}

// ���x����ύX
void CPlayer::ChangeLevel(int level)
{
	// �X�e�[�^�X�̃e�[�u���̃C���f�b�N�X�l�ɕϊ�
	int index =Math::Clamp(level-1,0, PLAYER_LEVEL_MAX);
	// �ő�X�e�[�^�X�ɐݒ�
	mCharaMaxStatus = PLAYER_STATUS[index];
	// �c��̌o���l���L�����Ă���
	int remaiExp = mCharaStatus.exp;
	// ���݂̃X�e�[�^�X���ő�l�ɂ��邱�ƂŁAHP��
	mCharaStatus = mCharaMaxStatus;
	// �c��̌o���l�𔽉f
	mCharaStatus.exp = remaiExp;

	// HP��ݒ�
	mpGameUI->SetMaxHp(mCharaMaxStatus.hp);
	mpGameUI->SetHp(mCharaStatus.hp);

	// SP��ݒ�
	mpGameUI->SetMaxSp(mCharaMaxStatus.SpecialPoint);
	mpGameUI->SetSp(mCharaStatus.SpecialPoint);

	mpGameUI->SetAvoidMaxValue(200);
	mpGameUI->SetAvoidValue(mRollingTime);

	// �o���l��ݒ�
	mpGameUI->SetMaxExp(mCharaMaxStatus.exp);
	mpGameUI->SetExp(mCharaStatus.exp);

	// ���ݒl�̃X�e�[�^�X�̃X�P�[���l�𔽉f
	Scale(CVector::one * DEFAULT_SCALE * mCharaMaxStatus.volume);

	// ���݂̃��x���̃J�����̍�����ݒ�
	CCamera* mainCamera = CGameCamera2::MainCamera();
	if (mainCamera != nullptr)
	{
		CVector diff = DEFAULT_CAMERA_POS - mDefaultPos;
		diff.Normalize();
		diff.Y(diff.Y() + mCharaStatus.cameraHeight);
		mainCamera->SetFollowTargetOffset(diff);
	}
}

// HP�񕜂Ɠ���U��(SP)�̎�����
void CPlayer::AutomaticRecovery()
{
	mRecoveryAmount = mCharaStatus.level;
	// HP��
	mHealCount++;
	if (mCharaStatus.hp < mCharaMaxStatus.hp)
	{
		
		if (mHealCount > 700)
		{
			mCharaStatus.hp += mRecoveryAmount;
			mHealCount = 0;
		}
	}
	else if(mCharaStatus.hp > mCharaMaxStatus.hp)
	{
		mCharaStatus.hp = mCharaMaxStatus.hp;
	}
	if (mCharaStatus.hp <= 0)
	{
		mHealCount = 0;
	}
	if (mCharaStatus.hp == mCharaMaxStatus.hp)
	{
		mHealCount = 0;
	}

	// SP��
	if (mCharaStatus.SpecialPoint < mCharaMaxStatus.SpecialPoint)
	{
		mRecoveryCount++;
		if (mRecoveryCount > 400)
		{
			mCharaStatus.SpecialPoint++;
			mRecoveryCount = 0;
		}
	}
	if (mCharaStatus.SpecialPoint == mCharaMaxStatus.SpecialPoint)
	{
		mRecoveryCount = 0;
	}
}

// �U�����������������|�C���g(SP)����
void CPlayer::AttackRecovery()
{
	if (mCharaStatus.SpecialPoint < mCharaMaxStatus.SpecialPoint)
	{
		mCharaStatus.SpecialPoint++;
	}
}

// ����J�E���g
void CPlayer::RollingCount()
{
	if (mRollingCount >= 1)
	{
		mRollingCount--;
		ChangeState(EState::eRolling);
	}
	if (mRollingCount <= 0)
	{
		mRollingCount = 0;
	}
}

// �o���l�����Z
void CPlayer::AddExp(int exp)
{
	// ���݂̃X�e�[�^�X�Ɍo���l�����Z
	mCharaStatus.exp += exp;
	// ���̃��x���܂ł̌o���l���l�����Ă�����
	if (mCharaStatus.exp >= mCharaMaxStatus.exp)
	{
		// ���x���A�b�v
		LevelUp();
	}
}

// �X�V
void CPlayer::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;
	mHp = mCharaStatus.hp;
	mMaxHp = mCharaMaxStatus.hp;
	mSp = mCharaStatus.SpecialPoint;

	if (mAttackCount >= 1)
	{
		mAttackTime++;
	}

	// ��Ԃɍ��킹�āA�X�V������؂�ւ���
	switch (mState)
	{
		// �ҋ@���
		case EState::eIdle:
			UpdateIdle();
			break;
		// ���s
		case EState::eWalk:
			UpadateWalk();
			break;
		// �U��
		case EState::eAttack:
			UpdateAttack();
			break;
		// �U��2
		case EState::eAttack2:
			UpdateAttack2();
			break;
		// �U��3
		case EState::eAttack3:
			UpdateAttack3();
			break;
		// �U��4
		case EState::eAttack4:
			UpdateAttack4();
			break;
		// �U��5
		case EState::eAttack5:
			UpdateAttack5();
			break;
		// �U��6
		case EState::eAttack6:
			UpdateAttack6();
			break;
		// �U��7
		case EState::eAttack7:
			UpdateAttack7();
			break;
		// �U���I���҂�
		case EState::eAttackWait:
			UpdateAttackWait();
			break;
		// �W�����v�J�n
		case EState::eJumpStart:
			UpdateJumpStart();
			break;
		// �W�����v��
		case EState::eJump:
			UpdateJump();
			break;
		// �W�����v�I��
		case EState::eJumpEnd:
			UpdateJumpEnd();
			break;
	    // �U���̓A�b�v
		case EState::ePowerUp:
			UpdatePowerUp();
			break;
	    // �U���̓A�b�v�I��
		case EState::ePowerUpEnd:
			UpdatePowerUpEnd();
			break;
	    // �q�b�g
		case EState::eHit:
			UpdateHit();
			break;
		// �K�[�h
		case EState::eGuard:
			UpdateGuard();
			break;
		// �K�[�h�q�b�g
		case EState::eGuardHit:
			UpdateGuardHit();
			break;
		// ���
		case EState::eRolling:
			UpdateRolling();
			break;
		// ����
		case EState::eDie:
			UpdateDei();
			break;
		// �W�����v�U��
		case EState::eJumpAttack:
			UpdateJumpAttack();
			break;
		// �W�����v�U���I���҂�
		case EState::eJumpAttackWait:
			UpdateJumpAttackWait();
			break;
	}

	// �ҋ@���ƃW�����v���́A�ړ��������s��
	if (mState == EState::eIdle
		|| mState == EState::eJumpStart
		|| mState == EState::eJump
		|| mState == EState::eJumpEnd)
	{
		UpdateMove();
	}

	if (mState != EState::eJumpAttack)
	{
		mMoveSpeed -= CVector(0.0f, GRAVITY, 0.0f);
	}

	// �ړ�
	Position(Position() + mMoveSpeed * 60.0f * Time::DeltaTime());

	// �v���C���[���ړ������֌�����
	CVector current = VectorZ();
	CVector target = mMoveSpeed;
	target.Y(0.0f);
	target.Normalize();
	CVector forward = CVector::Slerp(current, target, 0.125f);
	Rotation(CQuaternion::LookRotation(forward));

	CVector dir = VectorX();
	dir.Y(0.0f);
	dir.Normalize();
	CVector AvoidanceGaugePos = Position() + dir * 15.0f;

	CAvoidanceGauge* avoidGauge = mpGameUI->GetAvoidGauge();
	if (mRollingCount < 1)
	{
		mRollingTime++;
		avoidGauge->SetWorldPos(AvoidanceGaugePos);
	}
	else
	{
		avoidGauge->SetShow(false);
	}
	if (mRollingTime >= 200)
	{
		mRollingCount++;
		mRollingTime = 0;
	}

	AutomaticRecovery();

	// �h��̓A�b�v��
	if (mDefenseUp == true)
	{
		mElapsedDefenseUpTime += Time::DeltaTime();
		// 20�b�o�߂�����h��̓A�b�vfalse�ɂ���||�h��̓A�b�v���Ԃ�0�ɂ���
		if (mElapsedDefenseUpTime >= 20.0f)
		{
			mElapsedDefenseUpTime = 0;
			mDefenseUp = false;
		}
	}
	else
	{
		mElapsedDefenseUpTime = 0;
	}

	// �񕜎�
	if (mHeal == true)
	{
		mpHealCircle->StartCircle();

		if (mCharaStatus.hp < mCharaMaxStatus.hp)
		{
			int Heal = 0;
			Heal = mCharaMaxStatus.hp * 0.5f;
			mCharaStatus.hp += Heal;
			mHeal = false;

			if (mCharaStatus.hp > mCharaMaxStatus.hp)
			{
				mCharaStatus.hp = mCharaMaxStatus.hp;
			}
		}
		if (mCharaStatus.hp == mCharaMaxStatus.hp || mCharaStatus.hp <= 0)
		{
			mHeal = false;
		}
	}

	// �U���̓A�b�v
	if (mPowerUp == true)
	{
		mElapsedPowerUpTime += Time::DeltaTime();

		// �U���̓A�b�v���Ȃ̂ŁA�o�t�T�[�N���̕\�����J�n
		mpBuffCircle->StartCircle();
		mpBuffAura->StartAura();

		switch (mBuffStep)
		{
		case 0:
			if (mElapsedPowerUpTime >= 0.0f)
			{
				mpPowerUpAura = new CPowerUpAura
				(
					0.0f,
					10.0f,
					VectorY(),
					40.0f,
					40.0f
				);
				mpPowerUpAura->SetOwner(this);
			}
			if (mElapsedPowerUpTime >= 0.3f)
			{
				mBuffStep++;
			}
			break;
		case 1:
			// �U���A�b�v���Ԃ��I������
			if (mElapsedPowerUpTime >= 10)
			{
				// �o�t�T�[�N���̕\�����I��
				mpBuffCircle->EndCircle();
				mpBuffAura->EndAura();

				mElapsedPowerUpTime = 0;
				mBuffStep = 0;
				mPowerUp = false;
			}
			break;
		}
	}
	else
	{
		mElapsedPowerUpTime = 0;
	}

	// �L�����N�^�[�̍X�V
	CXCharacter::Update();

	mIsGrounded = false;

	if (mState == EState::eHit)
	{
		SetColor(CColor(1.0f, 0.0f, 0.0f, 1.0f));
	}
	else
	{
		SetColor(CColor(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (mState == EState::eGuard)
	{
		if (mCharaStatus.hp <= 0)
		{
			ChangeState(EState::eDie);
		}
	}
	if (mCharaStatus.hp <= 0)
	{
		ChangeState(EState::eDie);
	}

	if (CInput::Key('1'))
	{
		if (CInput::PushKey(VK_UP)) mCharaStatus.hp++;
		else if (CInput::PushKey(VK_DOWN))
		{
			mCharaStatus.hp--;
			ChangeState(EState::eHit);
		}
	}
	else if (CInput::Key('2'))
	{
		mCharaStatus.exp++;
		if (mCharaStatus.exp == mCharaMaxStatus.exp)
		{
			LevelUp();
		}
	}
	else if (CInput::PushKey('3'))
	{
		ChangeLevel(81);
	}
	else if (CInput::PushKey('4'))
	{
		mPowerUp = true;
	}
	// �L�����N�^�[�̉����߂��R���C�_�[
	mpColliderSphereHead->Update();
	mpColliderSphereBody->Update();

	// �_���[�W���󂯂�R���C�_�[
	mpDamageColHead->Update();
	mpDamageColBody->Update();

	// ���݂�HP��ݒ�
	mpGameUI->SetHp(mCharaStatus.hp);

	// ���݂�SP��ݒ�
	mpGameUI->SetSp(mCharaStatus.SpecialPoint);

	// ���݂̃N�[���^�C����ݒ�
	mpGameUI->SetAvoidValue(mRollingTime);

	// ���݂̌o���l��ݒ�
	mpGameUI->SetExp(mCharaStatus.exp);

	// ���݂̃��x����ݒ�
	mpGameUI->SetPlayerLevel(mCharaStatus.level);

	//CDebugPrint::Print(" %.1fFPS( Delta:%f)\n", Time::FPS(), Time::DeltaTime());
}

// �Փˏ���
void CPlayer::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (self == mpColliderLine)
	{
		if (other->Layer() == ELayer::eField)
		{
			mMoveSpeed.Y(0.0f);
			Position(Position() + hit.adjust * hit.weight);
			mIsGrounded = true;

			if (other->Tag() == ETag::eRideableObject)
			{
				mpRideObject = other->Owner();
			}
		}
	}
	// ���̃L�����N�^�[�Ƃ̉����߂�����
	else if (self == mpColliderSphereHead || self == mpColliderSphereBody)
	{
		// �G�̎�
		if (other->Layer() == ELayer::eEnemy)
		{
			CVector pushBack = hit.adjust * hit.weight;
			pushBack.Y(0.0f);
			Position(Position() + pushBack);
		}

		// �|�[�V��������(�h��̓A�b�v)
		if (other->Tag() == ETag::ePortionBlue)
		{
			mDefenseUp = true;
		}
		// �|�[�V��������(��)
		if (other->Tag() == ETag::ePortionGreen)
		{
			if (mCharaStatus.hp < mCharaMaxStatus.hp)
			{
				mHeal = true;
			}
		}
		// �|�[�V��������(�U���̓A�b�v)
		if (other->Tag() == ETag::ePortionRed)
		{
			mPowerUp = true;
		}
	}
}

// �`��
void CPlayer::Render()
{
	CXCharacter::Render();
}

//��_���[�W����
void CPlayer::TakeDamage(int damage, CObjectBase* causedObj)
{
	if (mCharaStatus.hp <= 0)return;

	mCharaStatus.hp -= damage;
	
	if (mState == EState::eGuard)
	{
		ChangeState(EState::eGuardHit);
	}
	else
	{
		ChangeState(EState::eHit);
	}

	// �_���[�W���󂯂���A�ړ����~
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);

	// �U�������~
	mpSword->AttackEnd();
}

// �U���͂̋����������擾
float CPlayer::GetAtkBuff()const
{
	return mBaseAttackBuffRatio;
}

// �h��͂̋����������擾
float CPlayer::GetDefBuff(const CVector& attackDir)const
{
	// �K�[�h��Ԃł���΁A�h��2�{
	if (mState == EState::eGuard)
	{
		if (mDefenseUp == true)
		{
			float dot = CVector::Dot(attackDir.Normalized(), VectorZ());
			if (dot >= cosf(Math::DegreeToRadian(30.0f)))
			{
				return 4.0f;
			}
		}
		else
		{
			float dot = CVector::Dot(attackDir.Normalized(), VectorZ());
			if (dot >= cosf(Math::DegreeToRadian(30.0f)))
			{
				return 2.0f;
			}
		}
	}

	if (mDefenseUp == true)
	{
		return 2.0f;
	}

	// �ʏ펞�̖h��̊���
	return mBaseDefenseBuffRatio;
}