//�v���C���[�N���X�̃C���N���[�h
#include "CPlayer.h"
#include "CInput.h"
#include "CCamera.h"
#include "CHpGauge.h"
#include "CSaGauge.h"
#include "Maths.h"

// �v���C���[�̃C���X�^���X
CPlayer* CPlayer::spInstance = nullptr;

// �v���C���[�̃��f���f�[�^�̃p�X
#define MODEL_PATH "Character\\Player\\player.x"
#define MODEL_DOG "Character\\Player\\Dog.x"
//#define MODEL_SLIME "Character\\Slime\\Slime.x"
#define MODEL_TURTLE "Character\\Turtle\\Turtle.x"
//#define MODEL_MUSHROOM "Character\\Mushroom\\Mushroom.x"
#define HP_FRAME_IMAGE "Character\\Player\\HP\\HPframe.png"  //HP�t���[���摜
#define HP_IMAGE "Character\\Player\\HP\\HP.png"//HP�摜

// �v���C���[�̃A�j���[�V�����f�[�^�̃e�[�u��
const CPlayer::AnimData CPlayer::ANIM_DATA[] =
{
	//{ "",										true,	0.0f	},	// T�|�[�Y
	//{ "Character\\Player\\anim\\DogAttack.x",		true,	91.0f	},	// �ҋ@
	//{ "Character\\Player\\anim\\walk.x",		true,	66.0f	},	// ���s

	{ "Character\\Player\\animation\\DogIdle.x",	true,	221.0f	},  //�ҋ@ 221.0f
	{ "Character\\Player\\animation\\DogWalk.x",	true,	69.0f	},  //���s
	{ "Character\\Player\\animation\\DogAttack.x",	true,	91.0f	},  //�U��
	{ "Character\\Player\\animation\\DogJump.x",	true,	49.0f	},  //�W�����v
	//{ "Character\\Player\\animation\\DogPowerUp.x",	true,	143.0f	},  //�U���̓A�b�v
	{ "Character\\Player\\animation\\DogAttack2.x",	true,	140.0f	},  //�U��2
	//{ "Character\\Player\\animation\\DogImpact.x",	true,	43.0f	},  //�Ռ�
	//{ "Character\\Player\\animation\\DogDie.x",	true,	235.0f	},  //����

	//{ "Character\\Player\\anim\\jump_start.x",	false,	25.0f	},	// �W�����v�J�n
	//{ "Character\\Player\\anim\\jump.x",		true,	1.0f	},	// �W�����v��
	//{ "Character\\Player\\anim\\jump_end.x",	false,	26.0f	},	// �W�����v�I��
};

#define PLAYER_HEIGHT 2.0f

#define JUMP_SPEED 1.5f
#define GRAVITY 0.0625f
#define JUMP_END_Y 1.0f

#define HP 10  //HP
#define LEVEL 1  //���x��
#define SA 5   //����U��
#define POWER 5  //�U����
#define MOVE_SPEED 0.3f  //�ړ����x

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
	, recoverycount(0)
{

	//�C���X�^���X�̐ݒ�
	spInstance = this;

	// ���f���f�[�^�ǂݍ���
	CModelX* model = new CModelX();
	model->Load(MODEL_DOG);
	Scale(10.0f, 10.0f, 10.0f);

	//HP�Q�[�W���쐬
	mpHpGauge = new CHpGauge();
	mpHpGauge->SetPos(10.0f, 690.f);

	//SA�Q�[�W���쐬
	mpSaGauge = new CSaGauge();
	mpSaGauge->SetPos(10.0f,663.0f);

	//�ŏ���1���x���ɐݒ�
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
		this, ELayer::eField,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, PLAYER_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });

	/*mpColliderSphere = new CColliderSphere
	(
		this, ELayer::eField,
	);
	mpColliderSphere->SetCollisionLayers({ ELayer::eField });*/
}

CPlayer::~CPlayer()
{
	if (mpColliderLine != nullptr)
	{
		delete mpColliderLine;
		mpColliderLine = nullptr;
	}

	if (mpColliderSphere != nullptr)
	{
		delete mpColliderSphere;
		mpColliderSphere = nullptr;
	}

	if (mpModel != nullptr)
	{
		delete mpModel;
		mpModel = nullptr;
	}
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
		if (CInput::Key('W'))		input.Z(1.0f);
		else if (CInput::Key('S'))	input.Z(-1.0f);
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
}

// �U��
void CPlayer::UpdateAttack2()
{
	// �U���A�j���[�V�������J�n
	ChangeAnimation(EAnimType::eAttack2);
	// �U���I���҂���Ԃֈڍs
	mState = EState::eAttackWait;
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

//�U���̓A�b�v
void CPlayer::UpdatePowerUp()
{
	ChangeAnimation(EAnimType::ePowerUp);
	//�U���̓A�b�v�I��
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

//1���x���A�b�v
void CPlayer::LevelUp()
{
	int level = mCharaStatus.level;
	ChangeLevel(level + 1);
}

//���x����ύX
void CPlayer::ChangeLevel(int level)
{
	//�X�e�[�^�X�̃e�[�u���̃C���f�b�N�X�l�ɕϊ�
	int index =Math::Clamp(level-1,0, PLAYER_LEVEL_MAX);
	//�ő�X�e�[�^�X�ɐݒ�
	mCharaMaxStatus = PLAYER_STATUS[index];
	//���݂̃X�e�[�^�X���ő�l�ɂ��邱�ƂŁAHP��
	mCharaStatus = mCharaMaxStatus;

	mpHpGauge->SetMaxValue(mCharaMaxStatus.hp);
	mpHpGauge->SetValue(mCharaStatus.hp);

	mpSaGauge->SetMaxValue(mCharaStatus.SpecialAttack);
	mpSaGauge->SetValue(mCharaStatus.SpecialAttack);
}

//����U��(SA)�̎�����
void CPlayer::AutomaticRecovery()
{
	recoverycount++;
	if (mCharaStatus.level <= 4 && mCharaStatus.SpecialAttack < 3)
	{
		if (recoverycount > 300)
		{
			mCharaStatus.SpecialAttack++;
			recoverycount = 0;
		}
		else if (mCharaStatus.SpecialAttack == 2)
		{
			recoverycount = 0;
		}
	}
	if (mCharaStatus.level <= 9 && mCharaStatus.SpecialAttack < 5)
	{
		if (recoverycount > 300)
		{
			mCharaStatus.SpecialAttack++;
			recoverycount = 0;
		}
		else if (mCharaStatus.SpecialAttack == 4)
		{
			recoverycount = 0;
		}
	}
}

// �X�V
void CPlayer::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;

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
	}

	mMoveSpeed -= CVector(0.0f, GRAVITY, 0.0f);

	CDebugPrint::Print("%.1fFPS( Delta:%f)\n", Time::FPS(), Time::DeltaTime());

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

	//�v���C���[�̃f�o�b�N�\��
	static bool debug = false;
	if (CInput::PushKey('R'))
	{
		debug = !debug;
	}
	if(debug)
	{
		CDebugPrint::Print("  ���x��     %d\n", mCharaStatus.level);
		CDebugPrint::Print("   HP        %d / %d\n", mCharaStatus.hp,mCharaMaxStatus.hp);
		CDebugPrint::Print("  �U���l     %d\n",mCharaStatus.power);
		CVector scale = Scale();
		CDebugPrint::Print("  �X�P�[���l %f,%f,%f \n", scale.X(), scale.Y(), scale.Z());
		CDebugPrint::Print("  ����U��(SA)  %d / %d\n", mCharaStatus.SpecialAttack, mCharaMaxStatus.SpecialAttack);
	}
	if (CInput::Key('1'))
	{
		if (CInput::PushKey(VK_UP)) mCharaStatus.hp++;
		else if (CInput::PushKey(VK_DOWN)) mCharaStatus.hp--;
	}
	else if (CInput::PushKey('2'))
	{
		LevelUp();
	}
	//CDebugPrint::Print(" �X�e�[�^�X�m�F  R�{�^��");

	//HP�Q�[�W�Ɍ��݂�HP��ݒ�
	mpHpGauge->SetValue(mCharaStatus.hp);
	//SA�Q�[�W�Ɍ��݂�SA��ݒ�
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
			Position(Position() + hit.adjust);
			mIsGrounded = true;

			if (other->Tag() == ETag::eRideableObject)
			{
				mpRideObject = other->Owner();
			}
		}
	}

	if (self == mpColliderSphere)
	{
		if (other->Layer() == ELayer::eField)
		{
			mMoveSpeed.Y(0.0f);
			Position(Position() + hit.adjust);
			mIsGrounded = true;

			if (other->Tag() == ETag::eRideableObject)
			{
				mpRideObject = other->Owner();
			}
		}
	}
}

// �`��
void CPlayer::Render()
{
	CXCharacter::Render();
}
