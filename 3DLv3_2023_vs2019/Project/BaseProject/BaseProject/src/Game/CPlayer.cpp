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

// �v���C���[�̃��f���f�[�^�̃p�X
#define MODEL_PATH "Character\\Player\\player.x"
#define MODEL_DOG "Character\\Player\\Dog.x"
#define MODEL_TURTLE "Character\\Turtle\\Turtle.x"

// �v���C���[�̃A�j���[�V�����f�[�^�̃e�[�u��
const CPlayer::AnimData CPlayer::ANIM_DATA[] =
{
	//{ "",										true,	0.0f	},	// T�|�[�Y
	//{ "Character\\Player\\anim\\DogAttack.x",		true,	91.0f	},	// �ҋ@
	//{ "Character\\Player\\anim\\walk.x",		true,	66.0f	},	// ���s

	{ "Character\\Player\\animation\\DogIdle.x",	true,	221.0f	},  // �ҋ@ 221.0f
	{ "Character\\Player\\animation\\DogWalk.x",	true,	69.0f	},  // ���s
	{ "Character\\Player\\animation\\DogAttack.x",	true,	91.0f	},  // �U��
	{ "Character\\Player\\animation\\DogJump.x",	true,	49.0f	},  // �W�����v
	{ "Character\\Player\\animation\\DogAttack2.x",	true,	140.0f	},  // �U��2
	{ "Character\\Player\\animation\\DogAttack3.x",	true,	91.0f	},  // �U��3
	{ "Character\\Player\\animation\\DogAttack4.x",	true,	105.0f	},  // �U��4
	{ "Character\\Player\\animation\\DogPowerUp.x",	true,	143.0f	},  // �U���̓A�b�v
	{ "Character\\Player\\animation\\DogHit.x",	true,	60.0f	},      // �q�b�g 43.0f
	//{ "Character\\Player\\animation\\DogImpact.x",	true,	43.0f	},  // �Ռ�
	//{ "Character\\Player\\animation\\DogDie.x",	true,	235.0f	},  // ����

	//{ "Character\\Player\\anim\\jump_start.x",	false,	25.0f	},	// �W�����v�J�n
	//{ "Character\\Player\\anim\\jump.x",		true,	1.0f	},	// �W�����v��
	//{ "Character\\Player\\anim\\jump_end.x",	false,	26.0f	},	// �W�����v�I��
};

#define PLAYER_HEIGHT 2.0f

#define JUMP_SPEED 1.5f
#define GRAVITY 0.0625f
#define JUMP_END_Y 1.0f

#define MOVE_SPEED 0.3f  //�ړ����x

//�f�t�H���g�̃X�P�[���l
#define DEFAULT_SCALE 10.0f

int CPlayer::mPower;

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
	, healcount(0)
	, recoverycount(0)
{

	// �C���X�^���X�̐ݒ�
	spInstance = this;

	// ���f���f�[�^�ǂݍ���
	CModelX* model = new CModelX();
	model->Load(MODEL_DOG);
	// �f�t�H���g�X�P�[���̔��f
	Scale(CVector::one * DEFAULT_SCALE);

	// HP�Q�[�W���쐬
	mpHpGauge = new CHpGauge();
	mpHpGauge->SetPos(10.0f, 690.f);

	// SA�Q�[�W���쐬
	mpSaGauge = new CSaGauge();
	mpSaGauge->SetPos(10.0f,663.0f);

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
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, PLAYER_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });

	// �L�����N�^�[���m�̉����߂��R���C�_�[
	mpColliderSphere = new CColliderSphere
	(
		this,ELayer::ePlayer,
		0.4f
	);
	mpColliderSphere->SetCollisionLayers({ ELayer::eEnemy,ELayer::eEnemy2,ELayer::eExp });

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
	mpDamageCol->Position(-0.05f, 0.7f, 0.0f);

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
}

CPlayer::~CPlayer()
{
	SAFE_DELETE(mpColliderLine);
	SAFE_DELETE(mpColliderSphere);
	SAFE_DELETE(mpDamageCol);
	SAFE_DELETE(mpModel);
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
	
	if (mIsGrounded)
	{
		// �ړ�����
		// �L�[�̓��̓x�N�g�����擾
		CVector input;
		if (CInput::Key('W'))		input.Z(-1.0f);
		else if (CInput::Key('S'))	input.Z(1.0f);
		if (CInput::Key('A'))		input.X(-1.0f);
		else if (CInput::Key('D'))	input.X(1.0f);

		// ���̓x�N�g���̒����œ��͂���Ă��邩����
		if (input.LengthSqr() > 0.0f)
		{
			// �J�����̌����ɍ��킹���ړ��x�N�g���ɕϊ�
			CVector move = CCamera::MainCamera()->Rotation() * input;
			move.Y(0.0f);
			move.Normalize();

			mMoveSpeed += move * MOVE_SPEED * mCharaStatus.mobility;

			// ���s�A�j���[�V�����ɐ؂�ւ�
			ChangeAnimation(EAnimType::eWalk);
		}
		// �ړ��L�[����͂��Ă��Ȃ�
		else
		{
			// �ҋ@�A�j���[�V�����ɐ؂�ւ�
			ChangeAnimation(EAnimType::eIdle);
		}

		// ���N���b�N�ōU����Ԃֈڍs
		if (CInput::PushKey(VK_LBUTTON))
		{
			mMoveSpeed.X(0.0f);
			mMoveSpeed.Z(0.0f);
			mState = EState::eAttack;
		}
		// SPACE�L�[�ŃW�����v�J�n�ֈڍs
		else if (CInput::PushKey(VK_SPACE))
		{
			mState = EState::eJumpStart;
		}
		if (CInput::PushKey(VK_RBUTTON))
		{
			mMoveSpeed.X(0.0f);
			mMoveSpeed.Z(0.0f);
			//	mState = EState::ePowerUp;
		}
		if (mCharaStatus.SpecialAttack > 0)
		{
			if (CInput::PushKey('K'))
			{
				mMoveSpeed.X(0.0f);
				mMoveSpeed.Z(0.0f);
				mState = EState::eAttack2;
				mCharaStatus.SpecialAttack--;
			}
		}
	}
	else
	{
		// �ҋ@�A�j���[�V�����ɐ؂�ւ�
		ChangeAnimation(EAnimType::eIdle);
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

// �U���I���҂�
void CPlayer::UpdateAttackWait()
{
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
		mState = EState::eIdle;
	}
}

// 1���x���A�b�v
void CPlayer::LevelUp()
{
	int level = mCharaStatus.level;
	ChangeLevel(level + 1);
}

// ���x����ύX
void CPlayer::ChangeLevel(int level)
{
	// �X�e�[�^�X�̃e�[�u���̃C���f�b�N�X�l�ɕϊ�
	int index =Math::Clamp(level-1,0, PLAYER_LEVEL_MAX);
	// �ő�X�e�[�^�X�ɐݒ�
	mCharaMaxStatus = PLAYER_STATUS[index];
	// ���݂̃X�e�[�^�X���ő�l�ɂ��邱�ƂŁAHP��
	mCharaStatus = mCharaMaxStatus;
	mCharaStatus.exp = 0;

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
		diff.Y(diff.Y() + mCharaStatus.cameraHeight);
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

// �X�V
void CPlayer::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;
	mPower = mCharaStatus.power;

	// ��Ԃɍ��킹�āA�X�V������؂�ւ���
	switch (mState)
	{
		// �ҋ@���
		case EState::eIdle:
			UpdateIdle();
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
	}

	mMoveSpeed -= CVector(0.0f, GRAVITY, 0.0f);

	CDebugPrint::Print(" %.1fFPS( Delta:%f)\n", Time::FPS(), Time::DeltaTime());
	CDebugPrint::Print(" �v���C���[��HP�񕜎��� %d\n", healcount);

	// �ړ�
	Position(Position() + mMoveSpeed * 60.0f * Time::DeltaTime());

	// �v���C���[���ړ������֌�����
	CVector current = VectorZ();
	CVector target = mMoveSpeed;
	target.Y(0.0f);
	target.Normalize();
	CVector forward = CVector::Slerp(current, target, 0.125f);
	Rotation(CQuaternion::LookRotation(forward));

	AutomaticRecovery();

	// �L�����N�^�[�̍X�V
	CXCharacter::Update();

	mIsGrounded = false;

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
		CDebugPrint::Print("   Lv:     %d\n", mCharaStatus.level);
		CDebugPrint::Print("   EXP:     %d  / %d\n", mCharaStatus.exp, mCharaMaxStatus.exp);
		CDebugPrint::Print("   HP:     %d / %d\n", mCharaStatus.hp,mCharaMaxStatus.hp);
		CDebugPrint::Print(" �U����    %d\n",mCharaStatus.power);
		CDebugPrint::Print(" ����U��(SA)  %d / %d\n", mCharaStatus.SpecialAttack, mCharaMaxStatus.SpecialAttack);
		CVector scale = Scale();
		CDebugPrint::Print(" �X�P�[���l %f,%f,%f \n", scale.X(), scale.Y(), scale.Z());
	}

	if (debug2)
	{
		CDebugPrint::Print(" R:�X�e�[�^�X�m�F\n");
		CDebugPrint::Print(" 2�L�[�Ń��x���A�b�v\n");
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
	else if (CInput::PushKey('2'))
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
		CVector pushBack = hit.adjust * hit.weight;
		pushBack.Y(0.0f);
		Position(Position() + pushBack);
		if (other->Layer() == ELayer::eExp)
		{
			mCharaStatus.exp++;
			if (mCharaStatus.exp == mCharaMaxStatus.exp)
			{
				LevelUp();
			}
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
		mState = EState::eHit;
	}
}
