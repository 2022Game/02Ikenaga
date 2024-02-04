//�v���C���[�N���X�̃C���N���[�h
#include "CPlayer.h"
#include "CInput.h"
#include "CCamera.h"
#include "CHpGauge.h"
#include "CSaGauge.h"
#include "Maths.h"
#include "CSword.h"
#include "CShield.h"

// �v���C���[�̃C���X�^���X
CPlayer* CPlayer::spInstance = nullptr;
int CPlayer::mHp;

// �v���C���[�̃��f���f�[�^�̃p�X
#define MODEL_PATH "Character\\Player\\player.x"

#define MODEL_TURTLE "Character\\Turtle\\Turtle.x"

// �v���C���[�̃A�j���[�V�����f�[�^�̃e�[�u��
const CPlayer::AnimData CPlayer::ANIM_DATA[] =
{
	//{ "",										true,	0.0f	},	// T�|�[�Y
	//{ "Character\\Player\\anim\\DogAttack.x",		true,	91.0f	},	// �ҋ@
	//{ "Character\\Player\\anim\\walk.x",		true,	66.0f	},	// ���s

	{ "Character\\Player\\animation\\DogIdle.x",	true,	221.0f	},  // �ҋ@ 221.0f
	{ "Character\\Player\\animation\\DogWalk.x",	true,	69.0f	},  // ���s
	{ "Character\\Player\\animation\\DogAttack.x",	true,   90.0f	},  // �U�� 91.0f
	{ "Character\\Player\\animation\\DogJump.x",	true,	49.0f	},  // �W�����v
	{ "Character\\Player\\animation\\DogAttack2.x",	true,	140.0f	},  // �U��2
	{ "Character\\Player\\animation\\DogAttack3.x",	true,	91.0f	},  // �U��3
	{ "Character\\Player\\animation\\DogAttack4.x",	true,	105.0f	},  // �U��4 105.0f
	{ "Character\\Player\\animation\\DogAttack5.x",	true,	101.0f	},  // �U��5 101.0f
	{ "Character\\Player\\animation\\DogAttack6.x",	true,	219.0f	},  // �U��6
	{ "Character\\Player\\animation\\DogAttack7.x",	true,	190.0f	},  // �U��7 213.0f
	{ "Character\\Player\\animation\\DogPowerUp.x",	true,	143.0f	},  // �U���̓A�b�v
	{ "Character\\Player\\animation\\DogHit.x",	true,	43.0f	},      // �q�b�g 43.0f
	{ "Character\\Player\\animation\\DogGuard.x",	false,	47.0f	},      // �K�[�h 47.0f
	{ "Character\\Player\\animation\\DogGuardHit.x",	false,	47.0f	},  // �K�[�h�q�b�g 47.0f
	{ "Character\\Player\\animation\\DogRolling.x",	true,	43.0f	},      // ��� 43.0f
	//{ "Character\\Player\\animation\\DogImpact.x",	true,	43.0f	},  // �Ռ�
	{ "Character\\Player\\animation\\DogDie.x",	true,	235.0f	},  // ����
};

#define PLAYER_HEIGHT 2.0f

#define JUMP_SPEED 1.5f
#define GRAVITY 0.0625f
#define JUMP_END_Y 1.0f

#define MOVE_SPEED 0.3f  //�ړ����x

//�f�t�H���g�̃X�P�[���l
#define DEFAULT_SCALE 10.0f

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
	,mRollingTime(0)
	,mRollingCount(1)
	,mAttackTime(0)
	,mAttackCount(0)
	, healcount(0)
	, recoverycount(0)
	, mDefaultPos(CVector::zero)
	, mIsPlayedSlashSE(false)
{
	// �C���X�^���X�̐ݒ�
	spInstance = this;

	// ���f���f�[�^�ǂݍ���
	CModelX* model = CResourceManager::Get<CModelX>("Player");
	// �f�t�H���g�X�P�[���̔��f
	Scale(CVector::one * DEFAULT_SCALE);

	// HP�Q�[�W���쐬
	mpHpGauge = new CHpGauge(false);
	mpHpGauge->SetPos(10.0f, 63.0f);
	mpHpGauge->SetShow(true);

	// SA�Q�[�W���쐬
	mpSaGauge = new CSaGauge();
	mpSaGauge->SetPos(10.0f,103.5f);

	// �ŏ���1���x���ɐݒ�
	ChangeLevel(11);

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
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, PLAYER_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });

	// �L�����N�^�[���m�̉����߂��R���C�_�[
	mpColliderSphere = new CColliderSphere
	(
		this,ELayer::ePlayer,
		0.5f
	);
	mpColliderSphere->SetCollisionLayers({ ELayer::eEnemy,ELayer::eEnemy2 });
	mpColliderSphere->Position(0.0f,0.4f,0.1f);

	///�_���[�W���󂯂�R���C�_�[���쐬
	mpDamageCol = new CColliderSphere
	(
		this, ELayer::eDamageCol,
		0.4f,
		false
	);
	//�_���[�W���󂯂�R���C�_�[��
	//�Փ˔�����s���R���C�_�[�̃��C���[�ƃ^�O��ݒ�
	mpDamageCol->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol->SetCollisionTags({ ETag::eEnemy });
	//�_���[�W���󂯂�R���C�_�[��������ւ��炷
	mpDamageCol->Position(-0.05f, 0.5f, 0.0f);

	//�f�t�H���g���W��ݒ�
	mDefaultPos = Position();

	//���𐶐����ĉE��Ɏ�������
	mpSword = new CSword();
	mpSword->SetAttachMtx(GetFrameMtx("Armature_mixamorig_RightHand"));
	mpSword->SetOwner(this);

	//���𐶐����č���Ɏ�������
	mpShield= new CShield();
	mpShield->SetAttachMtx(GetFrameMtx("Armature_mixamorig_LeftHand"));
	mpShield->SetOwner(this);

	mpSlashSE = CResourceManager::Get<CSound>("SlashSound");
}

CPlayer::~CPlayer()
{
	SAFE_DELETE(mpColliderLine);
	SAFE_DELETE(mpColliderSphere);
	SAFE_DELETE(mpDamageCol);
}

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


// �ҋ@
void CPlayer::UpdateIdle()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	int speed = 1.2f;
	int speed2 = -1.2f;

	if (mIsGrounded)
	{
		// �ړ�����
		// �L�[�̓��̓x�N�g�����擾
		CVector input;
		if (CInput::Key('W'))
		{
			// SPACE�L�[�ŉ��
			if (CInput::PushKey(VK_SHIFT) && mRollingCount >= 1)
			{
				RollingCount();
				mMoveSpeed.Z(-0.3f);
			}
			else
			{
				input.Z(speed2);
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
			if (CInput::PushKey(VK_SHIFT) && mRollingCount >= 1)
			{
				RollingCount();
				mMoveSpeed.X(-0.2f);
			}
			else
			{
				input.X(speed2);
			}
		}
		else if (CInput::Key('D'))
		{
			if (CInput::PushKey(VK_SHIFT) && mRollingCount >= 1)
			{
				RollingCount();
				mMoveSpeed.X(0.2f);
			}
			else
			{
				input.X(speed);
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

			// ���s�A�j���[�V�����ɐ؂�ւ�
			ChangeAnimation(EAnimType::eWalk);
			mpDamageCol->SetEnable(true);
		}
		// �ړ��L�[����͂��Ă��Ȃ�
		else
		{
			// �ҋ@�A�j���[�V�����ɐ؂�ւ�
			ChangeAnimation(EAnimType::eIdle);
			mpDamageCol->SetEnable(true);
		}

		// ���N���b�N�ōU����Ԃֈڍs
		if (CInput::PushKey(VK_LBUTTON))
		{
			mAttackCount++;
			mMoveSpeed.X(0.0f);
			mMoveSpeed.Z(0.0f);
			mState = EState::eAttack;
			if (mAttackCount == 2)
			{
				mMoveSpeed.X(0.0f);
				mMoveSpeed.Z(0.0f);
				mState = EState::eAttack5;
			}
			if (mAttackCount == 3)
			{
				mMoveSpeed.X(0.0f);
				mMoveSpeed.Z(0.0f);
				mState = EState::eAttack4;
			}
		}
		if (mAttackTime >= 300 ||mAttackCount <=0)
		{
			mAttackCount = 0;
			mAttackTime = 0;
		}
		
		// �X�y�V�����U��
		if (mCharaStatus.SpecialAttack >= 4)
		{
			if (CInput::PushKey(VK_MBUTTON))
			{
				mState = EState::eAttack7;
				mCharaStatus.SpecialAttack -= 4;
			}
		}

		if (CInput::Key(VK_RBUTTON))
		{
			mState = EState::eGuard;
		}
		if (CInput::PushKey(VK_SPACE))
		{
			mState = EState::eJumpStart;
		}
	}
	else
	{
		// �ҋ@�A�j���[�V�����ɐ؂�ւ�
		ChangeAnimation(EAnimType::eIdle);
		mpDamageCol->SetEnable(true);
	}
}

// ���s
void CPlayer::UpadateWalk()
{
	ChangeAnimation(EAnimType::eWalk);
	if (IsAnimationFinished())
	{
		mState = EState::eIdle;
	}
}

// �U��
void CPlayer::UpdateAttack()
{
	// �U���A�j���[�V�������J�n
	ChangeAnimation(EAnimType::eAttack);

	// �U���I���҂���Ԃֈڍs
	mState = EState::eAttackWait;

	//���ɍU���J�n��`����
	mpSword->AttackStart();

	// �a��SE�̍Đ��ς݃t���O��������
	mIsPlayedSlashSE = false;
}

// �U��2
void CPlayer::UpdateAttack2()
{
	// �U���A�j���[�V�������J�n
	ChangeAnimation(EAnimType::eAttack2);

	// �U���I���҂���Ԃֈڍs
	mState = EState::eAttackWait;

	//���ɍU���J�n��`����
	mpSword->AttackStart();
}

// �U��3
void CPlayer::UpdateAttack3()
{
	// �U���A�j���[�V�������J�n
	ChangeAnimation(EAnimType::eAttack3);
	// �U���I���҂���Ԃֈڍs
	mState = EState::eAttackWait;

	//���ɍU���J�n��`����
	mpSword->AttackStart();
}

// �U��4
void CPlayer::UpdateAttack4()
{
	// �U���A�j���[�V�������J�n
	ChangeAnimation(EAnimType::eAttack4);
	// �U���I���҂���Ԃֈڍs
	mState = EState::eAttackWait;

	//���ɍU���J�n��`����
	mpSword->AttackStart();
}

// �U��5
void CPlayer::UpdateAttack5()
{
	// �U���A�j���[�V�������J�n
	ChangeAnimation(EAnimType::eAttack5);
	// �U���I���҂���Ԃֈڍs
	mState = EState::eAttackWait;

	//���ɍU���J�n��`����
	mpSword->AttackStart();
}

// �U��6
void CPlayer::UpdateAttack6()
{
	// �U���A�j���[�V�������J�n
	ChangeAnimation(EAnimType::eAttack6);
	// �U���I���҂���Ԃֈڍs
	mState = EState::eAttackWait;

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
		mState = EState::eAttackWait;

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
		mState = EState::eIdle;
		ChangeAnimation(EAnimType::eIdle);

		//���ɍU���I����`����
		mpSword->AttackEnd();
	}
}

// �W�����v�J�n
void CPlayer::UpdateJumpStart()
{
	ChangeAnimation(EAnimType::eJumpStart);
	mState = EState::eJump;

	mMoveSpeed += CVector(0.0f, JUMP_SPEED, 0.0f);
	mIsGrounded = false;
}

// �W�����v��
void CPlayer::UpdateJump()
{
	if (mMoveSpeed.Y() <= 0.0f)
	{
		//ChangeAnimation(EAnimType::eJumpEnd);
		mState = EState::eJumpEnd;
	}
}

// �W�����v�I��
void CPlayer::UpdateJumpEnd()
{
	if (IsAnimationFinished())
	{
		mState = EState::eIdle;
	}
}

// �U���̓A�b�v
void CPlayer::UpdatePowerUp()
{
	ChangeAnimation(EAnimType::ePowerUp);
	// �U���̓A�b�v�I��
	mState = EState::ePowerUpEnd;
}

void CPlayer::UpdatePowerUpEnd()
{
	// �U���A�b�v�̃A�j���[�V�������I��������A
	if (IsAnimationFinished())
	{
		// �ҋ@��Ԃֈڍs
		mState = EState::eIdle;
		ChangeAnimation(EAnimType::eIdle);
	}
}

// �q�b�g
void CPlayer::UpdateHit()
{
	ChangeAnimation(EAnimType::eHit);
	if (IsAnimationFinished())
	{
		if (mCharaStatus.hp <= 0)
		{
			mState = EState::eDie;
		}
		else
		{
			mState = EState::eIdle;
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
		mState = EState::eIdle;
	}
}

// �K�[�h���̃q�b�g
void CPlayer::UpdateGuardHit()
{
	ChangeAnimation(EAnimType::eGuardHit);

	if (CInput::Key('G'))
	{
		mState = EState::eGuard;
	}
	else
	{
		mState = EState::eIdle;
	}
}

// ���
void CPlayer::UpdateRolling()
{
	ChangeAnimation(EAnimType::eRolling);
	Position(Position() + mMoveSpeed * 60.0f * Time::DeltaTime());
	RollingCount();
	if (IsAnimationFinished())
	{
		mState = EState::eIdle;
	}
	else
	{
		mpDamageCol->SetEnable(false);
	}
}

// ����
void CPlayer::UpdateDei()
{
	ChangeAnimation(EAnimType::eDie);
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
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

	mpHpGauge->SetMaxValue(mCharaMaxStatus.hp);
	mpHpGauge->SetValue(mCharaStatus.hp);

	mpSaGauge->SetMaxValue(mCharaStatus.SpecialAttack);
	mpSaGauge->SetValue(mCharaStatus.SpecialAttack);

	// ���ݒl�̃X�e�[�^�X�̃X�P�[���l�𔽉f
	Scale(CVector::one * DEFAULT_SCALE * mCharaMaxStatus.volume);

	// ���݂̃��x���̃J�����̍�����ݒ�
	CCamera* mainCamera = CCamera::MainCamera();
	if (mainCamera != nullptr)
	{
		CVector diff = DEFAULT_CAMERA_POS - mDefaultPos;
		diff.Y(diff.Y() +   mCharaStatus.cameraHeight);
		diff.Z(diff.Z() + mCharaStatus.cameraHeight);
		mainCamera->SetFollowTargetOffset(diff);
	}
}

// HP�񕜂Ɠ���U��(SA)�̎�����
void CPlayer::AutomaticRecovery()
{
	healcount++;
	recoverycount++;
	if (mCharaStatus.hp < mCharaMaxStatus.hp)
	{
		if (healcount > 700)
		{
			mCharaStatus.hp++;
			healcount = 0;
		}
	}
	if (mCharaStatus.hp <= 0)
	{
		healcount = 0;
	}
	if (mCharaStatus.hp == mCharaMaxStatus.hp)
	{
		healcount = 0;
	}
	if (mCharaStatus.SpecialAttack < mCharaMaxStatus.SpecialAttack)
	{
		if (recoverycount > 400)
		{
			mCharaStatus.SpecialAttack++;
			recoverycount = 0;
		}
	}
	if (mCharaStatus.SpecialAttack == mCharaMaxStatus.SpecialAttack)
	{
		recoverycount = 0;
	}
}

// ����J�E���g
void CPlayer::RollingCount()
{
	if (mRollingCount >= 1)
	{
		mRollingCount--;
		mState = EState::eRolling;
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
	}

	mMoveSpeed -= CVector(0.0f, GRAVITY, 0.0f);

	//CDebugPrint::Print(" �����: %d\n", mRollingCount);
	//CDebugPrint::Print(" �v���C���[��HP�񕜎��� %d\n", healcount);
	//CDebugPrint::Print(" �U���v�� %d\n", mAttackTime);
	//CDebugPrint::Print(" �U�������� %d\n", mAttackcount);

	// �ړ�
	Position(Position() + mMoveSpeed * 60.0f * Time::DeltaTime());

	// �v���C���[���ړ������֌�����
	CVector current = VectorZ();
	CVector target = mMoveSpeed;
	target.Y(0.0f);
	target.Normalize();
	CVector forward = CVector::Slerp(current, target, 0.125f);
	Rotation(CQuaternion::LookRotation(forward));

	if (mRollingCount < 1)
	{
		mRollingTime++;
	}
	if (mRollingTime >= 300)
	{
		mRollingCount++;
		mRollingTime = 0;
	}

	AutomaticRecovery();

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
			mState = EState::eDie;
		}
	}
	if (mCharaStatus.hp <= 0)
	{
		mState = EState::eDie;
	}

	// �v���C���[�̃f�o�b�N�\��
	static bool debug = false;
	static bool debug2 = true;
	if (CInput::PushKey('R'))
	{
		debug = !debug;
		debug2 = !debug2;
	}

	if(debug)
	{
		CDebugPrint::Print("    Lv:      %d\n", mCharaStatus.level);
		CDebugPrint::Print("   Exp:     %d  / %d\n", mCharaStatus.exp, mCharaMaxStatus.exp);
		CDebugPrint::Print("    Hp:    %d / %d\n", mCharaStatus.hp,mCharaMaxStatus.hp);
		CDebugPrint::Print(" �U����:    %d\n",mCharaStatus.power/2);
		CDebugPrint::Print(" �h���:    %d\n", mCharaStatus.defense);
		CDebugPrint::Print("    SA:    %d / %d\n", mCharaStatus.SpecialAttack, mCharaMaxStatus.SpecialAttack);
		CVector scale = Scale();
		CDebugPrint::Print(" �X�P�[���l %f,%f,%f \n", scale.X(), scale.Y(), scale.Z());
		CDebugPrint::Print(" �����: %d\n", mRollingCount);
	}

	if (debug2)
	{
		CDebugPrint::Print(" R: �X�e�[�^�X�\��\n");
		CDebugPrint::Print(" 2: ���x���A�b�v\n");
		CDebugPrint::Print(" WASD: �ړ�\n");
		CDebugPrint::Print(" G: �K�[�h\n");
		CDebugPrint::Print(" H�����N���b�N: �U��\n");
		CDebugPrint::Print(" K(���x��5�ȍ~:�g�p��): �X�y�V�����U��\n");
		CDebugPrint::Print(" J: �W�����v\n");
		CDebugPrint::Print(" WASD+�X�y�[�X: ���\n");
		CDebugPrint::Print(" ����̃N�[���^�C��: %d\n", mRollingTime);
		CDebugPrint::Print(" %.1fFPS( Delta:%f)\n", Time::FPS(), Time::DeltaTime());
	}

	if (CInput::Key('1'))
	{
		if (CInput::PushKey(VK_UP)) mCharaStatus.hp++;
		else if (CInput::PushKey(VK_DOWN))
		{
			mCharaStatus.hp--;
			mState = EState::eHit;
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
		ChangeLevel(100);
	}

	// HP�Q�[�W�Ɍ��݂�HP��ݒ�
	mpHpGauge->SetValue(mCharaStatus.hp);
	// SA�Q�[�W�Ɍ��݂�SA��ݒ�
	mpSaGauge->SetValue(mCharaStatus.SpecialAttack);
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
	else if (self == mpColliderSphere)
	{
		if (other->Layer() == ELayer::eEnemy || other->Layer() == ELayer::eEnemy2)
		{
			CVector pushBack = hit.adjust * hit.weight;
			pushBack.Y(0.0f);
			Position(Position() + pushBack);
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
	//���S���Ă�����A�_���[�W�͎󂯂Ȃ�
	//if (mCharaStatus.hp <= 0)return;

	//HP����_���[�W������
	//mCharaStatus.hp = max(mCharaStatus.hp - damage, 0);
	//mCharaStatus.hp -= damage;
	if (mCharaStatus.hp -= damage)
	{
		if (mState == EState::eGuard)
		{
			mState = EState::eGuardHit;
		}
		else
		{
			mState = EState::eHit;
		}
	}

	// �_���[�W���󂯂���A�ړ����~
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);

	// �U�������~
	mpSword->AttackEnd();
}


float CPlayer::GetAtkBuff()const
{
	return mBaseAttackBuffRatio;
}

float CPlayer::GetDefBuff(const CVector& attackDir)const
{
	// �K�[�h��Ԃł���΁A�h��2�{
	if (mState == EState::eGuard)
	{
		float dot = CVector::Dot(attackDir.Normalized(),VectorZ());
		if (dot >= cosf(Math::DegreeToRadian(30.0f)))
		{
			return 2.0f;
		}
	}

	// �ʏ펞�̖h��̊���
	return mBaseDefenseBuffRatio;
}